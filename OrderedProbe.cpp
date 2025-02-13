#ifndef ORDEREDPROBE_H
#define ORDEREDPROBE_H

#include <utility>
#include <algorithm>
#include <random>

#include "graph.cpp"
#include "utility.cpp"

class oprobe{
public:
    Graph* g;

    std::string ranking_function;

    std::vector<int>* order0;
    std::vector<int>* order1;
    std::vector<bool> is_prime;

    std::vector<std::vector<int>*> ProbedPrimeG;
    std::vector<std::vector<int>*> RemainedG;

    std::vector<double> ubs;
    std::vector<double> lbs;

    int first_prime_edge = 0;

    void TieShuffle(std::string ranking){
        
        if(ranking != "core_static") return;

        //std::cout<<"shuffling start ***********"<<std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());

        // std::cout<<"order0:"; for(unsigned int i=0;i<order0->size();i++) std::cout<<order0->at(i)<<" "; std::cout<<std::endl;
        // std::cout<<"order1:"; for(unsigned int i=0;i<order1->size();i++) std::cout<<order1->at(i)<<" "; std::cout<<std::endl;
        // std::cout<<"_prime:"; for(unsigned int i=0;i<is_prime.size();i++) std::cout<<is_prime[i]<<" "; std::cout<<std::endl;

        // std::cout<<"CoreEdgeNum:";
        // for(unsigned int i=0;i<g->CoreEdgeNum.size();i++) std::cout<<g->CoreEdgeNum[i]<<" "; std::cout<<std::endl;

        std::vector<int> distinct_core_edge_num;
        distinct_core_edge_num.push_back(g->CoreEdgeNum[0]);
        for(unsigned int i=0;i<g->CoreEdgeNum.size();i++)
            if(g->CoreEdgeNum[i] < distinct_core_edge_num[distinct_core_edge_num.size()-1])
                distinct_core_edge_num.push_back(g->CoreEdgeNum[i]);
        
        // std::cout<<"distinct_core_edge_num:";
        // for(unsigned int i=0;i<distinct_core_edge_num.size();i++)
        //     std::cout<<distinct_core_edge_num[i]<<" ";
        // std::cout<<std::endl;

        for(unsigned int i=0;i<distinct_core_edge_num.size()-1;i++)
            distinct_core_edge_num[i] -= distinct_core_edge_num[i+1];
        
        // std::cout<<"distinct_core_edge_num:";
        // for(unsigned int i=0;i<distinct_core_edge_num.size();i++)
        //     std::cout<<distinct_core_edge_num[i]<<" ";
        // std::cout<<std::endl;

        std::vector<std::vector<std::pair<std::pair<int, int>, bool>>*> core_separated;
        for(unsigned int i=0;i<distinct_core_edge_num.size();i++) core_separated.push_back(new std::vector<std::pair<std::pair<int, int>, bool>>());
        
        int count = 0;
        for(int i=distinct_core_edge_num.size()-1;i >= 0;i--){
            int temp_count = distinct_core_edge_num[i];
            for(unsigned int j=0;j<temp_count;j++){
                int s = order0->at(count);
                int t = order1->at(count);
                core_separated[distinct_core_edge_num.size() - i - 1]->push_back({{s, t}, is_prime[count]});
                count++;
            }
        }

        // std::cout<<"core_separated:"<<std::endl;
        // for(unsigned int i=0;i<core_separated.size();i++){
        //     std::cout<<"core_separated["<<i<<"]: ";
        //     for(unsigned int j=0;j<core_separated[i]->size();j++)
        //         std::cout<<"("<<core_separated[i]->at(j).first.first<<","<<core_separated[i]->at(j).first.second<<","<<core_separated[i]->at(j).second<<") ";
        //     std::cout<<std::endl;
        // }

        for(unsigned int i=0;i<core_separated.size();i++)
            std::shuffle(core_separated[i]->begin(), core_separated[i]->end(), gen);
        
        // std::cout<<"shuffled_core_separated:"<<std::endl;
        // for(unsigned int i=0;i<core_separated.size();i++){
        //     std::cout<<"shuffled_core_separated["<<i<<"]: ";
        //     for(unsigned int j=0;j<core_separated[i]->size();j++)
        //         std::cout<<"("<<core_separated[i]->at(j).first.first<<","<<core_separated[i]->at(j).first.second<<","<<core_separated[i]->at(j).second<<") ";
        //     std::cout<<std::endl;
        // }

        order0->clear();
        order1->clear();
        is_prime.clear();
        for(unsigned int i=0;i<core_separated.size();i++){
            for(unsigned int j=0;j<core_separated[i]->size();j++){
                order0->push_back(core_separated[i]->at(j).first.first);
                order1->push_back(core_separated[i]->at(j).first.second);
                is_prime.push_back(core_separated[i]->at(j).second);
            }
        }

        //std::cout<<"shuffling complete ***********"<<std::endl;
        
        // std::cout<<"shuffled_order0:"; for(unsigned int i=0;i<order0->size();i++) std::cout<<order0->at(i)<<" "; std::cout<<std::endl;
        // std::cout<<"shuffled_order1:"; for(unsigned int i=0;i<order1->size();i++) std::cout<<order1->at(i)<<" "; std::cout<<std::endl;
        // std::cout<<"shuffled_prime :"; for(unsigned int i=0;i<is_prime.size();i++) std::cout<<is_prime[i]<<" "; std::cout<<std::endl;
    }

    void print_ProbedPrimeG(){
        std::cout<<"ProbedPrimeG:"<<std::endl;
        for(unsigned int i=0;i<ProbedPrimeG.size();i++){
            if(!ProbedPrimeG[i]->empty()) {
                std::cout<<i<<" : ";
                for (int j : *ProbedPrimeG[i]) std::cout<< j <<" ";
                std::cout<<std::endl;
            }
        }
        std::cout<<std::endl;
    }

    void print_RemainedG(){
        std::cout<<"RemainedG:"<<std::endl;
        for(unsigned int i=0;i<RemainedG.size();i++){
            if(!RemainedG[i]->empty()) {
                std::cout<<i<<" : ";
                for (int j : *RemainedG[i]) std::cout<< j <<" ";
                std::cout<<std::endl;
            }
        }
        std::cout<<std::endl;
    }

    void print_is_prime(){
        std::cout<<"is_prime : ";
        for (auto &&i : is_prime) std::cout<< i <<" ";
        std::cout<<std::endl;
    }

    void print_order(){
        std::cout<<"order: ";
        for(unsigned int i=0;i<order0->size();i++)
            std::cout<<"("<<order0->at(i)<<","<<order1->at(i)<<")"<<" ";
        std::cout<<std::endl;
    }

    explicit oprobe(Graph* _g, std::string _rfunction){
        this->g = _g;

        this->ranking_function = std::move(_rfunction);

        this->order0 = new std::vector<int>();
        this->order1 = new std::vector<int>();
        for(unsigned int i=0;i<g->EdgeList.size();i++)
        {
            ProbedPrimeG.push_back(new std::vector<int>());
            RemainedG.push_back(new std::vector<int>());
        }
    }

    oprobe(Graph* _g, std::vector<int>* _order0, std::vector<int>* _order1, std::string _rfunction){
        this->g = _g;

        this->ranking_function = std::move(_rfunction);

        this->order0 = _order0;
        this->order1 = _order1;
        for(unsigned int i=0;i<order0->size();i++){
            int s = order0->at(i);
            int t = order1->at(i);
            unsigned int tmp=0;
            for(;tmp<g->EdgeList[s]->size();tmp++)
                if(g->EdgeList[s]->at(tmp) == t) break;
            if(g->PrimeEdgeList[s]->at(tmp))
                is_prime.push_back(true);
            else is_prime.push_back(false);
        }
        for(unsigned int i=0;i<g->EdgeList.size();i++)
        {
            ProbedPrimeG.push_back(new std::vector<int>());
            RemainedG.push_back(new std::vector<int>());
        }

        for(;first_prime_edge < is_prime.size();first_prime_edge++)
            if(is_prime[first_prime_edge]) break;
    }

    ~oprobe(){
        for (auto &i : ProbedPrimeG) delete i;
        for (auto &i : RemainedG) delete i;
    }

    void SetRemainedG(){
        for (auto &i : RemainedG) i->clear();

        for(int i=0;i<this->is_prime.size();i++){
            if(is_prime[i]){
                int s = order0->at((unsigned int)i), t=order1->at((unsigned int)i);
                RemainedG[s]->push_back(t);
                RemainedG[t]->push_back(s);
            }
        }

        for(int i=this->is_prime.size()-1;i>=0;i--){
            if(!is_prime[i]){
                int s = order0->at((unsigned int)i), t=order1->at((unsigned int)i);
                RemainedG[s]->push_back(t);
                RemainedG[t]->push_back(s);
            }
        }
    }

    std::vector<double>* run_exp_coreexact_vary_pcost(double expand, double gap){
        auto epsilons = new std::vector<double>();

        SetRemainedG();
        double remained_density;
        double probed_prime_density = 0;

        int pnum = int(gap * g->EdgeNum);
        if(pnum == 0) pnum = 1;

        int current_edge = 0;

        std::string expand_str = std::to_string(expand).substr(0, 3);
        std::string rname = "_"+g->name+"_"+this->ranking_function+"_expand"+expand_str;

        while(current_edge < g->EdgeNum){
            if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;
            exupdate(current_edge, pnum);

            current_edge += pnum;
            pnum = int(ceil(pnum * expand)+0.000000001);

            auto remained_pair = core_based_dsd(&RemainedG, rname);
            auto prime_pair = core_based_dsd(&ProbedPrimeG, rname);

            remained_density = remained_pair.first;
            probed_prime_density = prime_pair.first;

            this->ubs.push_back(remained_density);
            this->lbs.push_back(probed_prime_density);

            double epsilon = probed_prime_density / remained_density;
            epsilons->push_back(epsilon);
            if(epsilon > 0.99999999) break;
        }
        return epsilons;
    }

    std::pair<std::vector<double>*, std::vector<double>*> run_exp_coreexact_vary_eps(double expand, double gap){
        std::vector<double> epsilons = {1.0, 0.98, 0.96, 0.94, 0.92, 0.9, 0.8, 0.7, 0.6, 0.5};
        auto running_times = new std::vector<double>(epsilons.size(), 0.0);
        auto probe_counts = new std::vector<double>(epsilons.size(), g->EdgeNum+1);

        SetRemainedG();
        double remained_density;
        double probed_prime_density = 0;
        double running_time = 0;

        int pnum = int(gap * g->EdgeNum);
        if(pnum == 0) pnum = 1;
        
        int current_edge = 0;

        std::string expand_str = std::to_string(expand).substr(0, 3);
        std::string rname = "_"+g->name+"_"+this->ranking_function+"_expand"+expand_str;

        while(true){
            if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;
            exupdate(current_edge, pnum);

            current_edge += pnum;
            pnum = int(ceil(pnum * expand)+0.000000001);

            auto remained_pair = core_based_dsd(&RemainedG, rname);
            auto prime_pair = core_based_dsd(&ProbedPrimeG, rname);

            remained_density = remained_pair.first;
            probed_prime_density = prime_pair.first;

            running_time += remained_pair.second;
            running_time += prime_pair.second;

            bool b = false;
            for(unsigned int i=0;i<epsilons.size();i++){
                double epsilon = epsilons[i];
                if(epsilon * remained_density < probed_prime_density + 0.000000001){
                    if(current_edge < probe_counts->at(i)){
                        probe_counts->at(i) = current_edge;
                        running_times->at(i) = running_time;
                    }
                    if(epsilon > 0.99999999) b = true;
                }
            }
            if(b) break;
        }
        return {probe_counts, running_times};
    }

    std::pair<int, double> run_exp_coreexact(double approx_ratio, double expand){
        /* calling this function when really execute the deterministic algorithm*/

        SetRemainedG();
        double remained_density = 1;
        double probed_prime_density = 0;
        double running_time = 0;

        int pnum = 1;
        int current_edge = 0;

        std::string apr_str = std::to_string(approx_ratio).substr(0, 3);
        std::string expand_str = std::to_string(expand).substr(0, 3);


        std::string rname = "_"+g->name+"_"+this->ranking_function+"_apr"+apr_str+"_expand"+expand_str;

        //print_order();
        //print_RemainedG();

        while(approx_ratio * remained_density > probed_prime_density + 0.000000001){

            //std::cout<<"pnum:"<<pnum<<std::endl;

            if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;

            exupdate(current_edge, pnum);

            //print_RemainedG();
            //print_ProbedPrimeG();

            auto remained_pair = core_based_dsd(&RemainedG, rname);
            auto prime_pair = core_based_dsd(&ProbedPrimeG, rname);

            remained_density = remained_pair.first;
            probed_prime_density = prime_pair.first;

            running_time += remained_pair.second;
            running_time += prime_pair.second;

            current_edge += pnum;
            pnum = int(ceil(pnum * expand)+0.000000001);
        }
        return {current_edge, running_time};
    }

    int run_exp(double approx_ratio, double expand){
        /* calling this function when really execute the deterministic algorithm*/

        SetRemainedG();
        double remained_density = 1;
        double probed_prime_density = 0;
        int pnum = 1;
        int current_edge = 0;

        //print_order();
        //print_RemainedG();

        while(approx_ratio * remained_density > probed_prime_density + 0.000000001){

            //std::cout<<"pnum:"<<pnum<<std::endl;

            if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;

            exupdate(current_edge, pnum);

            //print_RemainedG();
            //print_ProbedPrimeG();

            auto remained = exact_dsd(&RemainedG);
            delete remained.second;
            auto probed_prime = exact_dsd(&ProbedPrimeG);
            delete probed_prime.second;
            remained_density = remained.first;
            probed_prime_density = probed_prime.first;

            current_edge += pnum;
            pnum = int(ceil(pnum * expand)+0.000000001);
        }
        return current_edge;
    }

    void exupdate(int current_edge, int pnum){
        for(int i=current_edge;i<current_edge+pnum;i++){
            int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
            if(is_prime[i]){
                ProbedPrimeG[s]->push_back(t);
                ProbedPrimeG[t]->push_back(s);
            }
            else{
                RemainedG[s]->pop_back();
                RemainedG[t]->pop_back();
            }
        }
    }

    std::vector<double>* run_linear(double ratio){
        auto probed_prime_densities = new std::vector<double>();
        probed_prime_densities->push_back(0);
        SetRemainedG();
        int pnum = int(ratio * g->EdgeNum);
        int current_edge = 0;

        //print_order();
        //print_ProbedPrimeG();

        while(current_edge + pnum <= g->EdgeNum){
            exupdate(current_edge, pnum);

            //print_ProbedPrimeG();

            auto probed_prime = exact_dsd(&ProbedPrimeG);
            delete probed_prime.second;
            probed_prime_densities->push_back((probed_prime.first / g->PrimeDS));
            if(probed_prime_densities->at(probed_prime_densities->size()-1) >= 0.99999999) break;
            current_edge += pnum;
        }
        return probed_prime_densities;
    }

    int run_binary(double approx_ratio){
        /* using binary search over order0 to find the optimal solution based on predefined order;
         * cannot be used when really executing the deterministic algorithm*/

        //print_order();
        //print_is_prime();

        int lb = first_prime_edge;
        int ub = is_prime.size() - 1;
        int mid = (ub + lb) / 2;


        while(lb < ub){

            //std::cout<<"lb:"<<lb<<"  ub:"<<ub<<"  mid:"<<mid<<std::endl;
            update(mid);

            //print_ProbedPrimeG();
            //print_RemainedG();

            auto remained = exact_dsd(&RemainedG);
            delete remained.second;
            auto probed_prime = exact_dsd(&ProbedPrimeG);
            delete probed_prime.second;
            double remained_density = remained.first;
            double probed_prime_density = probed_prime.first;

            //std::cout<<"remained_density:"<<remained_density<<std::endl;
            //std::cout<<"probed_prime_density:"<<probed_prime_density<<std::endl;

            //if(remained_density - probed_prime_density > (approx_ratio * remained_density + 0.000000001)) {
            if(approx_ratio * remained_density > probed_prime_density + 0.000000001){
                lb = mid + 1;
                //std::cout<<"remained_density > probed_prime_density"<<std::endl;
            }
            else {
                ub = mid - 1;
                //std::cout<<"remained_density < probed_prime_density"<<std::endl;
            }
            mid = (ub + lb) / 2;
        }

        return (lb + 1);
    }

    void update(int mid){
        for(unsigned int i=0;i<g->EdgeList.size();i++){
            RemainedG[i]->clear();
            ProbedPrimeG[i]->clear();
        }

        for(int i=first_prime_edge;i<=mid;i++)
        {
            int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
            if(is_prime[i]){
                ProbedPrimeG[s]->push_back(t);
                ProbedPrimeG[t]->push_back(s);
                RemainedG[s]->push_back(t);
                RemainedG[t]->push_back(s);
            }
        }
        for(int i=mid + 1;i<is_prime.size();i++){
            int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
            RemainedG[s]->push_back(t);
            RemainedG[t]->push_back(s);
        }
    }
};

class soe : public oprobe{
public:
    std::vector<int> solid;
    std::vector<int> empty;
    std::vector<int> next_destination;
    better_priority_queue::updatable_priority_queue<int, int> solid_ub_priority_queue;

    void probe_next(unsigned int v){
        while(g->EdgeList[v]->size() > next_destination[v]
              && g->ProbeEdgeList[v]->at((unsigned int)next_destination[v]))
            next_destination[v]++;

        while(g->EdgeList[v]->size() > next_destination[v]
              && g->PrimeEdgeList[v]->at((unsigned int)next_destination[v])){
            int nbr = g->EdgeList[v]->at((unsigned int)next_destination[v]);
            solid[v]++;
            solid[(unsigned int)nbr]++;
            if(!g->ProbeEdgeList[v]->at((unsigned int)next_destination[v])){
                g->ProbeEdgeList[v]->at((unsigned int)next_destination[v]) = true;
                if(v < nbr) {
                    order0->push_back(v);
                    order1->push_back(nbr);
                }
                else{
                    order0->push_back(nbr);
                    order1->push_back(v);
                }
                is_prime.push_back(true);
            }
            for(unsigned int j=0;j<g->EdgeList[nbr]->size();j++){
                if(g->EdgeList[nbr]->at(j) == v){
                    if(!g->ProbeEdgeList[nbr]->at(j))
                        g->ProbeEdgeList[nbr]->at(j) = true;
                    break;
                }
            }

            while(g->EdgeList[v]->size() > next_destination[v]
                  && g->ProbeEdgeList[v]->at((unsigned int)next_destination[v]))
                next_destination[v]++;
            while(g->EdgeList[nbr]->size() > next_destination[(unsigned int)nbr]
                  && g->ProbeEdgeList[nbr]->at((unsigned int)next_destination[nbr]))
                next_destination[(unsigned int)nbr]++;
        }
        if(g->EdgeList[v]->size() > next_destination[v]){
            empty[v]++;
            int nbr = g->EdgeList[v]->at((unsigned int)next_destination[v]);
            empty[(unsigned int)nbr]++;
            if(!g->ProbeEdgeList[v]->at((unsigned int)next_destination[v])){
                g->ProbeEdgeList[v]->at((unsigned int)next_destination[v]) = true;
                if(v < nbr) {
                    order0->push_back(v);
                    order1->push_back(nbr);
                }
                else{
                    order0->push_back(nbr);
                    order1->push_back(v);
                }
                is_prime.push_back(false);
            }
            for(unsigned int j=0;j<g->EdgeList[nbr]->size();j++){
                if(g->EdgeList[nbr]->at(j) == v){
                    g->ProbeEdgeList[nbr]->at(j) = true;
                    break;
                }
            }
            next_destination[v]++;
            while(g->EdgeList[nbr]->size() > next_destination[(unsigned int)nbr]
                  && g->ProbeEdgeList[nbr]->at((unsigned int)next_destination[nbr]))
                next_destination[nbr]++;
            solid_ub_priority_queue.push(v, g->EdgeList[v]->size() - empty[v]);
            solid_ub_priority_queue.update(nbr, g->EdgeList[nbr]->size() - empty[nbr]);
        }
    }

    explicit soe(Graph* _g, std::string _rfunction) : oprobe(_g, _rfunction){
        this->g = _g;

        for(unsigned int i=0;i<g->EdgeList.size();i++){
            this->solid.push_back(0);
            this->empty.push_back(0);
            this->next_destination.push_back(0);
        }
        for(unsigned int i=0;i<this->g->EdgeList.size();i++)
            if(!g->EdgeList[i]->empty()) solid_ub_priority_queue.push(i, g->EdgeList[i]->size());

        // Initialize order0 & order1 & is_prime
        while(!solid_ub_priority_queue.empty()){
            int tmp_node = solid_ub_priority_queue.top().key;
            solid_ub_priority_queue.pop();
            if(next_destination[tmp_node] < g->EdgeList[tmp_node]->size())
                probe_next((unsigned int)tmp_node);
        }

        for(;first_prime_edge < is_prime.size();first_prime_edge++)
            if(is_prime[first_prime_edge]) break;
    }

    ~soe(){
        delete this->order0;
        delete this->order1;
    }
};

#endif