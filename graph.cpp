#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <math.h>
#include "updatable_priority_queue.h"
#include "dsd.cpp"

class Graph{
public:
    std::string name;

    std::vector<std::vector<int>*> EdgeList;
    int EdgeNum = 0;

    std::vector<int> csr0, csr1;
    std::vector<int> incremental_count;

    std::vector<std::vector<bool>*> ValidEdgeList;
    int ValidEdgeNum = 0;

    std::vector<std::vector<bool>*> ProbeEdgeList; // whether the edge has been propoed
    std::vector<std::vector<bool>*> PrimeEdgeList; // whether an edge belongs to hidden coreGraph

    int PrimeEdgeNum = 0;
    int PrimeNodeNum = 0;

    double PrimeDensity = -1;
    double PrimeDS = -1;

    std::vector<unsigned int> CoreNumber;
    std::vector<int> CoreEdgeNum;
    std::vector<int> CoreNodeNum;
    std::vector<int> CorePrimeEdgeNum;

    std::vector<unsigned int> CsrTrussNumber;

    double df_max = 0;
    std::vector<int> DFDNumber;
    std::vector<double> DFDensities;
    std::vector<int> DFEdgeNum;

    Graph(const std::string &dataset, bool sign){
        this->name = dataset;

        std::string link_file = "datasets/"+dataset + ".txt";
        std::ifstream link_in;
        link_in.open(link_file);
        std::string link_line;

        std::vector<std::vector<std::pair<int, bool>>*> CombinedEdgeList;

        while(getline(link_in, link_line)) {
            if (link_line[0] == '#') continue;
            std::stringstream ss0(link_line);
            int temp_count = 0;
            std::string temp_str;
            int s, t;
            bool p;
            while (std::getline(ss0, temp_str, '\t')) {
                temp_count++;
                if (temp_count == 1) s = stoi(temp_str);
                else if (temp_count == 2) t = stoi(temp_str);
                else if (temp_count == 3) {
                    p = (stoi(temp_str) == 1);
                } else break;
            }
            while (s >= CombinedEdgeList.size() || t >= CombinedEdgeList.size())
                CombinedEdgeList.push_back(new std::vector<std::pair<int, bool>>());
            CombinedEdgeList[s]->push_back({t, p});
            CombinedEdgeList[t]->push_back({s, p});
        }
        link_in.close();

        for(auto &i: CombinedEdgeList) std::sort(i->begin(), i->end());
        for(unsigned int i=0;i<CombinedEdgeList.size();i++){
            this->EdgeList.push_back(new std::vector<int>());
            this->PrimeEdgeList.push_back(new std::vector<bool>());
        }

        for(unsigned int i=0;i<CombinedEdgeList.size();i++){
            for(unsigned int j=0;j<CombinedEdgeList[i]->size();j++){
                int nbr = CombinedEdgeList[i]->at(j).first;
                bool pp = CombinedEdgeList[i]->at(j).second;
                if(j == 0 || nbr != CombinedEdgeList[i]->at(j-1).first){
                    this->EdgeList[i]->push_back(nbr);
                    this->PrimeEdgeList[i]->push_back(!(sign^pp));
                    EdgeNum++;
                }
            }
        }
        EdgeNum /= 2;
        for (auto &i : CombinedEdgeList) delete i;
        for(unsigned int i=0;i<this->EdgeList.size();i++){
            this->ProbeEdgeList.push_back(new std::vector<bool>());
            for(unsigned int j=0;j<this->EdgeList[i]->size();j++)
                this->ProbeEdgeList[i]->push_back(false);
        }
        for(auto &i : this->EdgeList)
            ValidEdgeList.push_back(new std::vector<bool>(i->size(), false));

        for(unsigned int i=0;i<EdgeList.size();i++)
            for(unsigned int j=0;j<EdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                if(nbr > i){
                    csr0.push_back(i);
                    csr1.push_back(nbr);
                }
            }
        int count = 0;
        incremental_count.push_back(0);
        for(unsigned int i=0;i<EdgeList.size();i++){
            for(unsigned int j=0;j<EdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                if(nbr > i) count++;
            }
            incremental_count.push_back(count);
        }
    }

    explicit Graph(const std::string &dataset){
        this->name = dataset;

        std::string link_file = "datasets/"+dataset + ".txt";
        std::ifstream link_in;
        link_in.open(link_file);
        std::string link_line;

        while(getline(link_in, link_line)){
            if(link_line[0] == '#') continue;
            std::stringstream ss0(link_line);
            int temp_count = 0;
            std::string temp_str;
            unsigned int s, t;
            while(std::getline(ss0, temp_str, '\t')){
                temp_count++;
                if(temp_count == 1) s = (unsigned int)stoi(temp_str);
                else if(temp_count == 2) t = (unsigned int) stoi(temp_str);
                else break;
            }
            while(s >= this->EdgeList.size() || t >= this->EdgeList.size())
                this->EdgeList.push_back(new std::vector<int>());

            this->EdgeList[s]->push_back(t);
            this->EdgeList[t]->push_back(s);
            EdgeNum++;
        }

        link_in.close();
        for (auto &i : this->EdgeList) std::sort(i->begin(), i->end());

        for(unsigned int i=0;i<this->EdgeList.size();i++){
            this->ProbeEdgeList.push_back(new std::vector<bool>());
            this->PrimeEdgeList.push_back(new std::vector<bool>());
        }
        for(unsigned int i=0;i<this->EdgeList.size();i++){
            for(unsigned int j=0;j<this->EdgeList[i]->size();j++){
                this->ProbeEdgeList[i]->push_back(false);
                this->PrimeEdgeList[i]->push_back(false);
            }
        }

        for (auto &i : EdgeList)
            ValidEdgeList.push_back(new std::vector<bool>(i->size(), false));

        for(unsigned int i=0;i<EdgeList.size();i++)
            for(unsigned int j=0;j<EdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                if(nbr > i){
                    csr0.push_back(i);
                    csr1.push_back(nbr);
                }
            }
        int count = 0;
        incremental_count.push_back(0);
        for(unsigned int i=0;i<EdgeList.size();i++){
            for(unsigned int j=0;j<EdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                if(nbr > i) count++;
            }
            incremental_count.push_back(count);
        }
    }

    void df(const std::string &dataset){
        for(unsigned int i=0;i<EdgeList.size();i++) DFDNumber.push_back(-1);

        std::vector<int> current_dfd;
        std::unordered_map<int, bool> current_dfd_map;
        std::string dfd_file = "datasets/"+dataset+".dfd";
        std::ifstream dfd_in;
        dfd_in.open(dfd_file);
        std::string dfd_line;

        int df_count = 0;

        while(getline(dfd_in, dfd_line)){
            std::stringstream ss0(dfd_line);
            int temp_count = 0;
            std::string temp_str;
            while(std::getline(ss0, temp_str, '\t')) {
                int dfd_node = stoi(temp_str);
                current_dfd.push_back(dfd_node);
                current_dfd_map.insert({dfd_node, true});
                DFDNumber[dfd_node] = df_count;
            }

            int dfd_edge_num = 0;
            for(int n: current_dfd){
                for (int nbr : *EdgeList[n]) {
                    if(current_dfd_map.find(nbr)!=current_dfd_map.end())
                        dfd_edge_num++;
                }
            }
            dfd_edge_num /= 2;

            DFEdgeNum.push_back(dfd_edge_num);

            double dfd_density = (dfd_edge_num * 1.0) / current_dfd.size();
            if(dfd_density > df_max) df_max = dfd_density;
            DFDensities.push_back(dfd_density);
            df_count++;
        }
    }

    void setPrime(std::unordered_map<unsigned int, bool>* community){
        double prime_node_num = 0;
        double prime_edge_num = 0;

        for(auto it = community->begin();it!= community->end();it++){
            int node = it->first;
            prime_node_num += 1;
            for(unsigned int i=0;i<this->EdgeList[node]->size();i++){
                auto nbr = (unsigned int)this->EdgeList[node]->at(i);
                if(community->find(nbr)!=community->end()) {
                    this->PrimeEdgeList[node]->at(i) = true;
                    prime_edge_num+=1.0;
                }
            }
        }
        this->PrimeDensity = prime_edge_num / (2*prime_node_num);
    }

    void clearProbeEdgeList(){
        for(auto &i : this->ProbeEdgeList)
            for(unsigned int j=0;j<i->size();j++)
                i->at(j) = false;
    }

    void clear(){
        for (auto &i : this->PrimeEdgeList)
            for(unsigned int j=0;j< i->size();j++)
                i->at(j) = false;
        this->PrimeDensity = -1;
        for(auto &i : this->ProbeEdgeList)
            for(unsigned int j=0;j<i->size();j++)
                i->at(j) = false;
        this->CorePrimeEdgeNum.clear();
    }

    double getPrimeDensity() { return this->PrimeDensity;}

    void getPrimeCoreRatio(){
        getPrimeDSD();
        int kvalue = (int)ceil(PrimeDS - 0.000000001);
        int kcore_edgenum = 0;
        int prime_kcore_edgenum = 0;
        for(unsigned int i=0;i<EdgeList.size();i++){
            for(unsigned int j=0;j<EdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                int corevalue = std::min(CoreNumber[i], CoreNumber[nbr]);
                if(corevalue >= kvalue){
                    kcore_edgenum++;
                    if(PrimeEdgeList[i]->at(j)) prime_kcore_edgenum++;
                }
            }
        }
        double ratio = (prime_kcore_edgenum * 1.0) / kcore_edgenum;
        std::cout<<"PrimeCoreRatio:"<<ratio<<std::endl;
    }

    void getPrimeDSD(){
        auto GTsubgraph = new std::vector<std::vector<int>*>();
        for(unsigned int i=0;i<EdgeList.size();i++) GTsubgraph->push_back(new std::vector<int>());
        for(unsigned int i=0;i<EdgeList.size();i++)
            for(unsigned int j=0;j<EdgeList[i]->size();j++)
                if(PrimeEdgeList[i]->at(j)) GTsubgraph->at(i)->push_back(EdgeList[i]->at(j));
        auto res = exact_dsd(GTsubgraph);
        delete res.second;
        for (auto &i : *GTsubgraph) delete i;
        delete GTsubgraph;
        this->PrimeDS = res.first;
    }

    double SuitableCoreEdgeRatio(){
        int kvalue = (int)ceil(this->PrimeDS-0.000000001);
        int core_edge_count = this->CoreEdgeNum[kvalue];
        double ratio = (core_edge_count * 1.0) / this->EdgeNum;
        return ratio;
    }

    int getPrimeEdgeNum(){
        this->PrimeEdgeNum = 0;
        for(unsigned int i=0;i<PrimeEdgeList.size();i++)
            for(unsigned int j=0;j<PrimeEdgeList[i]->size();j++)
                if(PrimeEdgeList[i]->at(j)) this->PrimeEdgeNum++;
        this->PrimeEdgeNum /= 2;
        return this->PrimeEdgeNum;
    }

    int getPrimeNodeNum(){
        this->PrimeNodeNum = 0;
        for(unsigned int i=0;i<PrimeEdgeList.size();i++)
        {   
            bool is_prime_node = false;
            for(unsigned int j=0;j<PrimeEdgeList[i]->size();j++)
                if(PrimeEdgeList[i]->at(j)) is_prime_node = true;
            if(is_prime_node) this->PrimeNodeNum++;
        }
        return this->PrimeNodeNum;
    }

    double getPrimeNodeEdgeRatio(){
        double avg_ratio = 0.0;
        double prime_node_num = 0.0;
        for(unsigned int i=0;i<PrimeEdgeList.size();i++){
            int node_prime_edge = 0;
            for(unsigned int j=0;j<PrimeEdgeList[i]->size();j++)
                if(PrimeEdgeList[i]->at(j)) node_prime_edge++;
            if(node_prime_edge > 0){
                prime_node_num += 1.0;
                avg_ratio += ((node_prime_edge * 1.0) / PrimeEdgeList[i]->size());
            }
        }
        avg_ratio /= prime_node_num;
        return avg_ratio;
    }

    void printEdgeList(){
        std::cout<<"EdgeList:"<<std::endl;
        for(unsigned int i=0;i<EdgeList.size();i++){
            std::cout<<i<<" : ";
            for (int j : *EdgeList[i]) std::cout<< j <<" ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    void printProbeEdgeList(){
        std::cout<<"ProbeEdgeList:"<<std::endl;
        for(unsigned int i=0;i<ProbeEdgeList.size();i++){
            std::cout<<i<<" : ";
            for (auto &&j : *ProbeEdgeList[i]) std::cout<< j <<" ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    void printPrimeEdgeList(){
        std::cout<<"PrimeEdgeList:"<<std::endl;
        for(unsigned int i=0;i<PrimeEdgeList.size();i++){
            std::cout<<i<<" : ";
            for (auto &&j : *PrimeEdgeList[i]) std::cout<< j <<" ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    void printPrimeEdges(){
        std::cout<<"PrimeEdges:"<<std::endl;
        for(unsigned int i=0;i<PrimeEdgeList.size();i++){
            std::cout<<i<<" : ";
            for(unsigned int j=0;j<PrimeEdgeList[i]->size();j++)
                if(PrimeEdgeList[i]->at(j)) std::cout<<"("<<i<<", "<<EdgeList[i]->at(j)<<") ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    void truss(){
        //print_csr();

        std::vector<int> remained_support;
        better_priority_queue::updatable_priority_queue<int, int> pQ;
        for(unsigned int i=0;i<csr0.size();i++) CsrTrussNumber.push_back(0);

        int count = 0;
        for(unsigned int node=0;node<EdgeList.size();node++){
            std::unordered_map<int, bool> node_nbr_map;
            for (int &j : *EdgeList[node]) node_nbr_map.insert({j, true});
            for(unsigned int j=0;j<EdgeList[node]->size();j++){
                int nbr = EdgeList[node]->at(j);
                if(nbr > node) {
                    int triangle_num = 0;
                    for (int nnbr : *EdgeList[nbr]) if (node_nbr_map.find(nnbr) != node_nbr_map.end()) triangle_num++;
                    remained_support.push_back(triangle_num);
                    pQ.push(count, -triangle_num);
                    count++;
                }
            }
        }

        //std::cout<<"remained_support:";
        //for (int i : remained_support) std::cout<< i <<" "; std::cout<<std::endl;
        //print_CsrTrussNumber();

        int current_truss_num = 0;

        while(!pQ.empty()){
            int next = pQ.pop_value().key;
            current_truss_num = std::max(current_truss_num, remained_support[next]);
            CsrTrussNumber[next] = (unsigned int)current_truss_num;

            remained_support[next] = 0;

            int s = csr0[next], t = csr1[next];

            //std::cout<<"next edge: ("<<s<<", "<<t<<")"<<std::endl;

            std::vector<int> common_neighbors;
            std::unordered_map<int, bool> s_nbr_map;
            for(unsigned int i=0;i<EdgeList[s]->size();i++) {
                int nnbr = EdgeList[s]->at(i);
                if (remained_support[get_csr_pos(s, nnbr)] > 0)
                    s_nbr_map.insert({nnbr, true});
            }
            for(unsigned int i=0;i<EdgeList[t]->size();i++){
                int nnbr = EdgeList[t]->at(i);
                if(remained_support[get_csr_pos(t, nnbr)] > 0 && s_nbr_map.find(nnbr)!=s_nbr_map.end())
                    common_neighbors.push_back(nnbr);
            }

            for(int nbr: common_neighbors){
                int temp_csr_pos = get_csr_pos(s, nbr);
                if(remained_support[temp_csr_pos] > 0){
                    remained_support[temp_csr_pos]--;
                    pQ.update(temp_csr_pos, -remained_support[temp_csr_pos]);
                }

                temp_csr_pos = get_csr_pos(t, nbr);
                if(remained_support[temp_csr_pos] > 0){
                    remained_support[temp_csr_pos]--;
                    pQ.update(temp_csr_pos, -remained_support[temp_csr_pos]);
                }
            }

            //std::cout<<"remained_support:";
            //for (int i : remained_support) std::cout<< i <<" "; std::cout<<std::endl;
            //print_CsrTrussNumber();
        }

        //print_CsrTrussNumber();
    }

    void core(){
        better_priority_queue::updatable_priority_queue<int, int> pQ;
        std::vector<unsigned int> remained_deg;
        for(unsigned int i=0;i<EdgeList.size();i++) CoreNumber.push_back(0);

        for(unsigned int i=0;i<EdgeList.size();i++){
            if(!EdgeList[i]->empty()){
                pQ.push(i, -EdgeList[i]->size());
                remained_deg.push_back(EdgeList[i]->size());
            }
            else remained_deg.push_back(0);
        }

        unsigned int current_core_num = 0;

        while(!pQ.empty()){
            int next = pQ.pop_value().key;
            current_core_num = std::max(current_core_num, remained_deg[next]);
            CoreNumber[next] = current_core_num;

            remained_deg[next] = 0;

            for(int nbr: *EdgeList[next]){
                if(remained_deg[nbr] > 0){
                    remained_deg[nbr] -= 1;
                    pQ.update(nbr, -remained_deg[nbr]);
                }
            }
        }

        //std::cout<<"CoreNumber:";
        //for(unsigned int i=0;i<CoreNumber.size();i++){
        //    std::cout<<CoreNumber[i]<<" ";
        //}
        //std::cout<<std::endl;

        get_core_edge_num();

        for(unsigned int i=0;i<CoreEdgeNum.size();i++) CoreNodeNum.push_back(0);
        for(unsigned int i: CoreNumber) CoreNodeNum[i]++;

        int temp = 0;
        for(int i=CoreNodeNum.size()-1;i>=0;i--){
            temp += CoreNodeNum[i];
            CoreNodeNum[i] = temp;
        }
    }

    void get_core_prime_edge_num(){
        for(unsigned int i=0;i<CoreEdgeNum.size();i++) CorePrimeEdgeNum.push_back(0);

        for(int v = 0; v < EdgeList.size();v++){
            for(int u=0;u<EdgeList[v]->size();u++){
                int nbr = EdgeList[v]->at((unsigned int)u);
                int core_value = std::min(CoreNumber[v], CoreNumber[nbr]);
                if(PrimeEdgeList[v]->at((unsigned int)u)){
                    CorePrimeEdgeNum[(unsigned int)core_value]++;
                }
            }
        }
        for (int &i : CorePrimeEdgeNum) i /= 2;
        int temp = 0;
        for(int i=CorePrimeEdgeNum.size()-1;i>=0;i--) {
            temp += CorePrimeEdgeNum[i];
            CorePrimeEdgeNum[i] = temp;
        }

        //std::cout<<"CorePrimeEdgeNum:";
        //for (int i : CorePrimeEdgeNum) std::cout<< i <<" ";
        //std::cout<<std::endl;
    }

    void get_core_edge_num(){
        for(unsigned int i=0;i<EdgeList.size();i++) CoreEdgeNum.push_back(0);

        for(int v = 0; v < EdgeList.size();v++){
            for(int u = 0; u<EdgeList[v]->size();u++){
                int nbr = EdgeList[v]->at((unsigned int)u);
                int core_value = std::min(CoreNumber[v], CoreNumber[nbr]);
                CoreEdgeNum[(unsigned int)core_value]++;
            }
        }
        for(unsigned int i=0;i<EdgeList.size();i++)
            CoreEdgeNum[i] /= 2;

        int temp = 0;
        for(int i=EdgeList.size()-1; i>=0; i--){
            temp += CoreEdgeNum[i];
            CoreEdgeNum[i] = temp;
        }

        while(CoreEdgeNum[CoreEdgeNum.size()-1]==0)
            CoreEdgeNum.pop_back();
    }

    void print_core_edge_num(){
        std::cout<<"CoreEdgeNum:";
        for (int i : CoreEdgeNum) std::cout<< i <<" ";
        std::cout<<std::endl;
    }

    void print_core_node_num(){
        std::cout<<"CoreNodeNum:";
        for(int i: CoreNodeNum) std::cout<< i <<" ";
        std::cout<<std::endl;
    }

    void set_valid_edge(int k){
        ValidEdgeNum = 0;
        for (auto &i : ValidEdgeList) for (auto &&j : *i) j = false;

        for(unsigned int i=0;i<ValidEdgeList.size();i++)
            for(unsigned int j=0;j<ValidEdgeList[i]->size();j++){
                int nbr = EdgeList[i]->at(j);
                if(CoreNumber[i] >= k && CoreNumber[nbr] >= k){
                    ValidEdgeList[i]->at(j) = true;
                    ValidEdgeNum++;
                }
            }
        ValidEdgeNum /= 2;
    }

    int get_csr_pos(int s, int nbr){
        int start, end;
        if(s > nbr) {
            start = nbr;
            end = s;
        }
        else{
            start = s;
            end = nbr;
        }

        int count = incremental_count[start];
        for(unsigned int i=0;i<EdgeList[start]->size();i++)
        {
            int nnbr = EdgeList[start]->at(i);
            if (nnbr > start)
                count++;
            if(nnbr == end) break;
        }
        count--;
        return count;
    }

    void print_csr(){
        std::cout<<"csr: ";
        for(unsigned int i=0;i<csr0.size();i++)
            std::cout<<"("<<csr0[i]<<","<<csr1[i]<<")"<<" ";
        std::cout<<std::endl;
    }

    void print_incremental_count(){
        std::cout<<"incremental_count: ";
        for (int i : incremental_count)
            std::cout<< i <<" ";
        std::cout<<std::endl;
    }

    void print_CsrTrussNumber(){
        std::cout<<"CsrTrussNumber: ";
        for(int i: CsrTrussNumber)
            std::cout<<i<<" ";
        std::cout<<std::endl;
    }

    void print_df(){
        std::cout<<"df_max:"<<df_max<<std::endl;
        std::cout<<"DFDNumber:";
        for (int i : DFDNumber) std::cout<< i <<" "; std::cout<<std::endl;
        std::cout<<"DFDensities:";
        for (double i : DFDensities) std::cout<< i <<" "; std::cout<<std::endl;
        std::cout<<"DFEdgeNum:";
        for(int i : DFEdgeNum) std::cout<< i <<" "; std::cout<<std::endl;
    }
};

#endif