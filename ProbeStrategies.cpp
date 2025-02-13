#include "graph.cpp"
#include <unordered_map>

class probe_strategy{
public:
    Graph *g;

    std::vector<std::vector<int>*> remained_score;

    std::vector<int> order0; // store the results of probe order (start node)
    std::vector<int> order1; // store the results of probe order (end node)

    better_priority_queue::updatable_priority_queue<int, int> pq;

    explicit probe_strategy(Graph* _g){
        this->g = _g;

        //g->print();

        for(unsigned int i=0;i<g->EdgeList.size();i++)
            this->remained_score.push_back(new std::vector<int>());
    }

    virtual void update_remained_score(int s, int t){}

    void run(){
        //print_csr();
        //print_incremental_count();
        //print_remained_score();

        while(!pq.empty()){
            int next = pq.pop_value().key;
            int s = g->csr0[next];
            int t = g->csr1[next];

            order0.push_back(s);
            order1.push_back(t);

            //std::cout<<"next: ("<<s<<", "<<t<<")"<<std::endl;
            update_remained_score(s, t);
            //print_remained_score();
            //print_order();
        }
    }

    void print_remained_score(){
        std::cout<<"remained_score:"<<std::endl;
        for(unsigned int i=0;i<remained_score.size();i++){
            std::cout<<i<<" : ";
            for (int j : *remained_score[i]) std::cout<< j <<" ";
            std::cout<<std::endl;
        }
    }

    void print_order(){
        std::cout<<"order: ";
        for(unsigned int i=0;i<order0.size();i++)
            std::cout<<"("<<order0[i]<<","<<order1[i]<<")"<<" ";
        std::cout<<std::endl;
    }

    ~probe_strategy(){
        for(unsigned int i=0;i<g->EdgeList.size();i++) {
            delete this->remained_score[i];
        }
    }
};

class core_static_probe : public probe_strategy{
public:
    explicit core_static_probe(Graph* _g) : probe_strategy(_g){
        g->core();

        int count = 0;
        for(unsigned int node=0;node < g->EdgeList.size();node++){
            for(unsigned int i=0;i<g->EdgeList[node]->size();i++){
                int nbr = g->EdgeList[node]->at(i);
                if(nbr > node){
                    int core_value = g->CoreNumber[node];
                    if(g->CoreNumber[nbr] < core_value) core_value = g->CoreNumber[nbr];
                    pq.push(count, core_value);
                    count++;
                }
            }
        }
    }

    void update_remained_score(int s, int t) override {}
};

class core_based_probe : public probe_strategy{
public:
    std::vector<std::vector<int>*> affected;
    std::vector<std::vector<bool>*> remaining_graph;

    explicit core_based_probe(Graph* _g) : probe_strategy(_g){
        g->core();

        int count = 0;
        for(unsigned int node=0;node < g->EdgeList.size();node++){
            for(unsigned int i=0;i<g->EdgeList[node]->size();i++){
                int nbr = g->EdgeList[node]->at(i);
                if(nbr > node){
                    int core_value = g->CoreNumber[node];
                    if(g->CoreNumber[nbr] < core_value) core_value = g->CoreNumber[nbr];
                    pq.push(count, core_value);
                    count++;
                }
            }
        }

        for(unsigned int i=0;i<g->EdgeList.size();i++)
            affected.push_back(new std::vector<int>());
        for(unsigned int i=0;i<g->EdgeList.size();i++)
            this->remaining_graph.push_back(new std::vector<bool>(g->EdgeList[i]->size(), true));
    }

    void update_remained_score(int s, int t) override {
        if(g->CoreNumber[s] == 1 && g->CoreNumber[t] == 1) return;

        unsigned int tmp = 0;
        for(;tmp<g->EdgeList[s]->size();tmp++) if(g->EdgeList[s]->at(tmp) == t) break;
        remaining_graph[s]->at(tmp) = false;

        tmp = 0;
        for(;tmp < g->EdgeList[t]->size();tmp++) if(g->EdgeList[t]->at(tmp) == s) break;
        remaining_graph[t]->at(tmp) = false;

        //print_core_value();

        //std::cout<<"affected_source_node:"<<s<<std::endl;

        std::vector<bool> visited(g->CoreNumber.size(), false);
        std::vector<int> induced_core;
        std::vector<int> active;

        if(g->CoreNumber[s] >= g->CoreNumber[t]){
            active.push_back(t);
            visited[t] = true;
        }
        if(g->CoreNumber[s] <= g->CoreNumber[t]){
            active.push_back(s);
            visited[s] = true;
        }

        while(!active.empty()){
            int n = active[active.size() - 1];
            active.pop_back();
            induced_core.push_back(n);
            for(unsigned int i=0;i<g->EdgeList[n]->size();i++)
                if(remaining_graph[n]->at(i)){
                    int nbr = g->EdgeList[n]->at(i);
                    if(!visited[nbr] && g->CoreNumber[nbr] >= g->CoreNumber[n]){
                        active.push_back(nbr);
                        visited[nbr] = true;
                    }
                }
        }

        //std::cout<<"induced_core:";
        //for (int i : induced_core) std::cout<< i <<" ";
        //std::cout<<std::endl;

        std::unordered_map<int, bool> induced_core_map;
        for(int n: induced_core)
            induced_core_map.insert({n, true});

        for(int n: induced_core){
            for(unsigned int i=0;i<g->EdgeList[n]->size();i++){
                int nbr = g->EdgeList[n]->at(i);
                if(remaining_graph[n]->at(i) && induced_core_map.find(nbr)!=induced_core_map.end())
                    affected[n]->push_back(nbr);
            }
        }

        //print_affected();

        better_priority_queue::updatable_priority_queue<int, int> kcore_pq;
        std::vector<unsigned int> remained_deg;
        for(unsigned int i=0;i<affected.size();i++){
            if(!affected[i]->empty()){
                kcore_pq.push(i, -affected[i]->size());
                remained_deg.push_back(affected[i]->size());
            }
            else remained_deg.push_back(0);
        }

        unsigned int current_core_num = 0;
        while(!kcore_pq.empty()){
            int next = kcore_pq.pop_value().key;
            current_core_num = std::max(current_core_num, remained_deg[next]);
            if(g->CoreNumber[next] > current_core_num) {
                g->CoreNumber[next] = current_core_num;

                for(unsigned int i=0;i<g->EdgeList[next]->size();i++)
                    if(remaining_graph[next]->at(i)){
                        int nbr = g->EdgeList[next]->at(i);
                        if(g->CoreNumber[next] < g->CoreNumber[nbr])
                            pq.update(g->get_csr_pos(next, nbr), g->CoreNumber[next]);
                    }
            }
            else break;

            remained_deg[next] = 0;

            for(int nbr: *affected[next]){
                if(remained_deg[nbr] > 0){
                    remained_deg[nbr] -= 1;
                    kcore_pq.update(nbr, -remained_deg[nbr]);
                }
            }
        }
        affected_clear();
    }

    ~core_based_probe(){
        for(unsigned int i=0;i<g->EdgeList.size();i++)
            delete affected[i];
    }

    void print_core_value(){
        std::cout<<"CoreNumber:";
        for(unsigned int i=0;i<g->CoreNumber.size();i++){
            std::cout<<g->CoreNumber[i]<<" ";
        }
        std::cout<<std::endl;
    }

    void affected_clear(){
        for(unsigned int i=0;i<affected.size();i++)
            affected[i]->clear();
    }

    void print_affected(){
        std::cout<<"affected:"<<std::endl;
        for(unsigned int i=0;i<affected.size();i++){
            if(affected[i]->size() > 0){
                std::cout<<"n"<<i<<" : ";
                for(unsigned int j=0;j<affected[i]->size();j++){
                    std::cout<<affected[i]->at(j)<<" ";
                }
                std::cout<<std::endl;
            }
        }
    }
};

class truss_static_probe : public probe_strategy{
public:
    explicit truss_static_probe(Graph* _g) : probe_strategy(_g){
        g->truss();
        for(unsigned int count=0;count<g->CsrTrussNumber.size();count++)
            pq.push(count, g->CsrTrussNumber[count]);
    }

    void update_remained_score(int s, int t) override {}
};

class triangle_static_probe : public probe_strategy{
public:
    explicit triangle_static_probe(Graph* _g) : probe_strategy(_g){
        int count = 0;
        for(unsigned int node=0;node<g->EdgeList.size();node++){
            std::unordered_map<int, bool> node_nbr_map;
            for (int &j : *g->EdgeList[node]) node_nbr_map.insert({j, true});
            for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                int nbr = g->EdgeList[node]->at(j);
                int triangle_num = 0;
                for(int nnbr : *g->EdgeList[nbr]) if(node_nbr_map.find(nnbr)!= node_nbr_map.end()) triangle_num++;
                remained_score[node]->push_back(triangle_num);
                if(nbr > node){
                    pq.push(count, triangle_num);
                    count++;
                }
            }
        }
    }

    void update_remained_score(int s, int t) override {}
};

class triangle_based_probe : public probe_strategy{ // probe edge according to the number of triangles containing it
public:
    explicit triangle_based_probe(Graph* _g) : probe_strategy(_g){
        int count = 0;
        for(unsigned int node=0;node<g->EdgeList.size();node++){
            std::unordered_map<int, bool> node_nbr_map;
            for (int &j : *g->EdgeList[node]) node_nbr_map.insert({j, true});
            for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                int nbr = g->EdgeList[node]->at(j);
                int triangle_num = 0;
                for(int nnbr : *g->EdgeList[nbr]) if(node_nbr_map.find(nnbr)!= node_nbr_map.end()) triangle_num++;
                remained_score[node]->push_back(triangle_num);
                if(nbr > node){
                    pq.push(count, triangle_num);
                    count++;
                }
            }
        }
    }

    void update_remained_score(int s, int t) override{
        unsigned int tmp = 0;
        for(;tmp<g->EdgeList[s]->size();tmp++) if(g->EdgeList[s]->at(tmp)==t) break;
        remained_score[s]->at(tmp) = 0;
        tmp = 0;
        for(;tmp < g->EdgeList[t]->size();tmp++) if(g->EdgeList[t]->at(tmp) == s) break;
        remained_score[t]->at(tmp) = 0;

        std::vector<int> common_neighbors;
        std::unordered_map<int, bool> s_nbr_map;
        for(unsigned int i=0;i<g->EdgeList[s]->size();i++)
            if(remained_score[s]->at(i) > 0) s_nbr_map.insert({g->EdgeList[s]->at(i), true});
        for(unsigned int i=0;i<g->EdgeList[t]->size();i++)
            if(remained_score[t]->at(i) > 0 && s_nbr_map.find(g->EdgeList[t]->at(i))!= s_nbr_map.end())
                common_neighbors.push_back(g->EdgeList[t]->at(i));

        for(int nbr: common_neighbors){
            tmp = 0;
            for(; tmp < g->EdgeList[s]->size();tmp++) if(g->EdgeList[s]->at(tmp) == nbr) break;
            if(remained_score[s]->at(tmp) > 0){
                remained_score[s]->at(tmp)--;
                pq.update(g->get_csr_pos(s, nbr), remained_score[s]->at(tmp));
            }

            tmp =0;
            for(; tmp < g->EdgeList[t]->size();tmp++) if(g->EdgeList[t]->at(tmp) == nbr) break;
            if(remained_score[t]->at(tmp) > 0) {
                remained_score[t]->at(tmp)--;
                pq.update(g->get_csr_pos(t, nbr), remained_score[t]->at(tmp));
            }

            tmp = 0;
            for(; tmp < g->EdgeList[nbr]->size();tmp++) if(g->EdgeList[nbr]->at(tmp) == s) break;
            if(remained_score[nbr]->at(tmp) > 0) remained_score[nbr]->at(tmp)--;

            tmp = 0;
            for(; tmp < g->EdgeList[nbr]->size();tmp++) if(g->EdgeList[nbr]->at(tmp) == t) break;
            if(remained_score[nbr]->at(tmp) > 0) remained_score[nbr]->at(tmp)--;
        }
    }
};

class degsum_static_probe : public probe_strategy{
public:
    explicit degsum_static_probe(Graph* _g) : probe_strategy(_g){
        int count = 0;
        for(unsigned int node=0;node<g->EdgeList.size();node++)
            for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                int nbr = g->EdgeList[node]->at(j);
                int temp_sum = g->EdgeList[node]->size() + g->EdgeList[nbr]->size();
                remained_score[node]->push_back(temp_sum);
                if(nbr > node){
                    pq.push(count, temp_sum);
                    count++;
                }
            }
    }

    void update_remained_score(int s, int t) override{}
};

class degsum_based_probe : public probe_strategy{ // probe the edge according to degree sum of its two end nodes
public:
    explicit degsum_based_probe(Graph* _g) : probe_strategy(_g){
        int count = 0;
        for(unsigned int node=0;node<g->EdgeList.size();node++)
            for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                int nbr = g->EdgeList[node]->at(j);
                int temp_sum = g->EdgeList[node]->size() + g->EdgeList[nbr]->size();
                remained_score[node]->push_back(temp_sum);
                if(nbr > node){
                    pq.push(count, temp_sum);
                    count++;
                }
            }
    }

    void update_remained_score(int s, int t) override{
        unsigned int tmp = 0;
        for(;tmp<g->EdgeList[s]->size();tmp++) if(g->EdgeList[s]->at(tmp)==t) break;
        remained_score[s]->at(tmp) = 0;
        tmp = 0;
        for(;tmp < g->EdgeList[t]->size();tmp++) if(g->EdgeList[t]->at(tmp) == s) break;
        remained_score[t]->at(tmp) = 0;

        for(unsigned int i=0;i<remained_score[s]->size();i++){
            int nbr = g->EdgeList[s]->at(i);
            if(remained_score[s]->at(i) > 0){

                //std::cout<<"s: "<<s << "  nbr:"<<nbr<<"  pos:"<<get_pos(s, nbr)<<std::endl;

                remained_score[s]->at(i)--;
                tmp = 0;
                for(; tmp <g->EdgeList[nbr]->size();tmp++) if(g->EdgeList[nbr]->at(tmp) == s) break;
                remained_score[nbr]->at(tmp)--;

                tmp = (unsigned int)g->get_csr_pos(s, nbr);
                pq.update(tmp, remained_score[s]->at(i));
            }
        }

        for(unsigned int i=0;i<remained_score[t]->size();i++){
            int nbr = g->EdgeList[t]->at(i);
            if(remained_score[t]->at(i) > 0){

                //std::cout<<"t: "<<t << "  nbr:"<<nbr<<"  pos:"<<get_pos(t, nbr)<<std::endl;

                remained_score[t]->at(i)--;
                tmp = 0;
                for(; tmp < g->EdgeList[nbr]->size();tmp++) if(g->EdgeList[nbr]->at(tmp) == t) break;
                remained_score[nbr]->at(tmp)--;

                tmp = (unsigned int)g->get_csr_pos(t, nbr);
                pq.update(tmp, remained_score[t]->at(i));
            }
        }
    }
};