#include "graph.cpp"
#include "utility.cpp"
#include "OrderedProbe.cpp"
#include <random>
#include <algorithm>
#include <ctime>
#include <chrono>

class sample_verification : public oprobe{
public:
    std::vector<std::vector<int>*> Rsubgraph;
    std::vector<std::vector<bool>*> sampled_matrix;
    // bool pre_kindex_update = false;

    explicit sample_verification(Graph* _g) : oprobe(_g, ""){
        for(unsigned int i=0;i<g->EdgeList.size();i++){
            Rsubgraph.push_back(new std::vector<int>());
            sampled_matrix.push_back(new std::vector<bool>(g->EdgeList[i]->size(), false));
        }
    }

    ~sample_verification(){
        for(unsigned int i=0;i<sampled_matrix.size();i++) delete sampled_matrix[i];
        for(unsigned int i=0;i<Rsubgraph.size();i++) delete Rsubgraph[i];
    };

    // double global_edge_sampling(double p, double ratio, double epsilon){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     for(unsigned int i=0;i<g->EdgeList.size();i++)
    //         for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
    //             int nbr = g->EdgeList[i]->at(j);
    //             if(nbr > i && g->ValidEdgeList[i]->at(j)){
    //                 if(interval(gen) < p){
    //                     if(g->PrimeEdgeList[i]->at(j)){
    //                         Rsubgraph[i]->push_back(nbr);
    //                         Rsubgraph[nbr]->push_back(i);
    //                     }
    //                 }
    //             }
    //         }
        
    //     auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //     delete Rsubgraph_dsd.second;
    //     double expected_dsd = Rsubgraph_dsd.first / p;

    //     double ub = expected_dsd * epsilon + ratio * sqrt(1/sample_ratio - 1);

    //     std::cout<<"ub:"<<ub<<std::endl;

    //     Rsubgraph_clear();
    //     return ub;
    // }

    // double global_edge_sampling(double p){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     for(unsigned int i=0;i<g->EdgeList.size();i++)
    //         for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
    //             int nbr = g->EdgeList[i]->at(j);
    //             if(nbr > i){
    //                 if(interval(gen) < p){
    //                     if(g->PrimeEdgeList[i]->at(j)){
    //                         Rsubgraph[i]->push_back(nbr);
    //                         Rsubgraph[nbr]->push_back(i);
    //                     }
    //                 }
    //             }
    //         }
        
    //     auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //     delete Rsubgraph_dsd.second;
    //     double expected_dsd = Rsubgraph_dsd.first / p;

    //     //std::cout<<"expected_dsd:"<<expected_dsd<<std::endl;
    //     double lb_diff = g->PrimeDS - expected_dsd;
    //     //std::cout<<"lb_diff:"<<lb_diff<<std::endl;

    //     Rsubgraph_clear();
    //     return lb_diff;
    // }

    // std::pair<int, double> edge_sampling_count_constant_stack(int pnum, int kindex, int pre_kindex, std::vector<int>* current_sample_num, std::vector<int>* diff_kvalues,
    //                                                 std::vector<int>* diff_shell_sizes, std::vector<int>* diff_core_sizes){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     int probe_count = 0;
    //     double sampling_prob = 0.0;

    //     std::vector<double> rnum, srnum;
    //     double lim;

    //     int count = 0;
    //     int k = diff_kvalues->at(kindex);
    //     int pre_k = diff_kvalues->at(pre_kindex);
        
    //     int shell_edge_num = 0;
    //     double shell_sampled_prob = 0.0;
    //     double core_sampled_prob = 0.0;

    //     if(pre_kindex == kindex){
    //         core_sampled_prob = (current_sample_num->at(kindex) * 1.0) / diff_core_sizes->at(kindex);
    //     }
    //     else{
    //         shell_edge_num = diff_core_sizes->at(kindex) - diff_core_sizes->at(pre_kindex);
    //         shell_sampled_prob = (current_sample_num->at(kindex) * 1.0) / shell_edge_num;
    //         core_sampled_prob = (current_sample_num->at(pre_kindex) * 1.0) / diff_core_sizes->at(pre_kindex);
    //     }

    //     if(pre_kindex > kindex && shell_sampled_prob < core_sampled_prob){
    //         std::cout<<"start shell sampling ***********"<<std::endl;
    //         bool core_sample_flag = false;

    //         std::cout<<"######################################"<<std::endl;//////////////////////////

    //         std::cout<<"shell_edge_num:"<<shell_edge_num<<std::endl;
    //         std::cout<<"current_sample_num->at(kindex):"<<current_sample_num->at(kindex)<<std::endl;

    //         unsigned int remained_shell_edge = shell_edge_num - current_sample_num->at(kindex);
    //         std::cout<<"remained_shell_edge:"<<remained_shell_edge<<std::endl;
    //         rnum.resize(remained_shell_edge);
    //         srnum.resize(remained_shell_edge);

            

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }
    //         std::sort(srnum.begin(), srnum.end());

    //         auto shell_plan_sample_num = (int)ceil(shell_edge_num * core_sampled_prob - 0.000000001);
    //         if(shell_plan_sample_num - current_sample_num->at(kindex) > pnum){
    //             lim = srnum[pnum];
    //         }
    //         else{
    //             if(shell_plan_sample_num - current_sample_num->at(kindex) < srnum.size())
    //                 lim = srnum[shell_plan_sample_num - current_sample_num->at(kindex)];
    //             else lim = 1.0;
    //             core_sample_flag = true;
    //         }

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     bool core_determ = (corevalue < pre_k && corevalue >= k);
    //                     if(core_determ && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
    //         if(shell_plan_sample_num - current_sample_num->at(kindex) >= pnum)
    //             current_sample_num->at(kindex) += pnum;
    //         else
    //             current_sample_num->at(kindex) += (shell_plan_sample_num - current_sample_num->at(kindex));
    //         shell_sampled_prob = (current_sample_num->at(kindex) * 1.0) / shell_edge_num;

    //         double resample_prob = shell_sampled_prob / core_sampled_prob;

    //         Rsubgraph_clear();

    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     bool core_determ = (corevalue < pre_k && corevalue >= k);
    //                     if(core_determ){
    //                         if(sampled_matrix[node]->at(j) && g->PrimeEdgeList[node]->at(j)){
    //                             Rsubgraph[node]->push_back(nbr);
    //                             Rsubgraph[nbr]->push_back(node);
    //                         }
    //                     }
    //                     else if(corevalue >= pre_k){
    //                         if(sampled_matrix[node]->at(j) && g->PrimeEdgeList[node]->at(j)){
    //                             if(interval(gen) < resample_prob){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         if(core_sample_flag){
    //             current_sample_num->at(kindex) += current_sample_num->at(pre_kindex);
    //             while(current_sample_num->size() > kindex + 1) current_sample_num->pop_back();
    //             sampling_prob = (current_sample_num->at(kindex) * 1.0) / diff_core_sizes->at(kindex);
    //             this->pre_kindex_update = true;
    //         }
    //         else (current_sample_num->at(kindex) * 1.0) / diff_shell_sizes->at(kindex);
    //     }
    //     else{
    //         std::cout<<"start core sampling ***********"<<std::endl;
    //         int remained_core_edge = diff_core_sizes->at(kindex) - current_sample_num->at(kindex);

    //         rnum.resize(remained_core_edge);
    //         srnum.resize(remained_core_edge);

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }
    //         std::sort(srnum.begin(), srnum.end());
            
    //         if(remained_core_edge <= pnum) lim = 1.0;
    //         else lim = srnum[pnum];

    //         std::cout<<"lim:"<<lim<<std::endl;

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     if(corevalue >= k && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                             if(g->PrimeEdgeList[node]->at(j)){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
    //         current_sample_num->at(kindex) += pnum;
    //         sampling_prob = (current_sample_num->at(kindex) * 1.0) / diff_core_sizes->at(kindex);
    //     }
    //     return {probe_count, sampling_prob};
    // }

    int edge_sampling_count_constant(double prob, std::vector<int>* current_sample_num){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> interval(0.0, 1.0);

        int probe_count = 0;
        std::vector<double> rnum, srnum;
        double lim;

        int remained_edge = g->EdgeNum - current_sample_num->at(0);

        rnum.resize(remained_edge);
        srnum.resize(remained_edge);
        for(unsigned int i=0;i<rnum.size();i++){
            rnum[i] = interval(gen);
            srnum[i] = rnum[i];
        }
        std::sort(srnum.begin(), srnum.end());

        lim = srnum[int(g->EdgeNum * prob - current_sample_num->at(0))];

        int count = 0;
        for(unsigned int node=0;node<g->EdgeList.size();node++)
            for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                int nbr = g->EdgeList[node]->at(j);
                if(nbr > node){
                    if(!sampled_matrix[node]->at(j)){
                        if(rnum[count] < lim){
                            if(!g->ProbeEdgeList[node]->at(j)){
                                probe_count++;
                                g->ProbeEdgeList[node]->at(j)=true;
                                unsigned int l=0;
                                for(;l<g->EdgeList[nbr]->size();l++)
                                    if(g->EdgeList[nbr]->at(l)==node) break;
                                g->ProbeEdgeList[nbr]->at(l)=true;
                            }
                            sampled_matrix[node]->at(j) = true;
                            current_sample_num->at(0)++;
                            unsigned int l=0;
                            for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
                            sampled_matrix[nbr]->at(l) = true;
                            if(g->PrimeEdgeList[node]->at(j)){
                                Rsubgraph[node]->push_back(nbr);
                                Rsubgraph[nbr]->push_back(node);
                            }
                        }
                        count++;
                    }
                }
            }
        return probe_count;
    }

    int edge_sampling_count_constant_stack(double prob, double last_prob, int kindex, int pre_kindex, std::vector<int>* current_sample_num, std::vector<int>* diff_kvalues, std::vector<int>* diff_shell_sizes, 
                                                                std::vector<int>* diff_core_sizes, bool core_sample){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> interval(0.0, 1.0);

        int probe_count = 0;
        std::vector<double> rnum, srnum;
        double lim;

        int count = 0;
        int k = diff_kvalues->at(kindex);
        int pre_k = diff_kvalues->at(pre_kindex);

        if(core_sample){
            // std::cout<<"core_sampling ********"<<std::endl;

            int remained_core_edge = diff_core_sizes->at(kindex) - current_sample_num->at(kindex);
            // std::cout<<"remained_core_edge:"<<remained_core_edge<<std::endl;
            
            rnum.resize(remained_core_edge);
            srnum.resize(remained_core_edge);

            for(unsigned int i=0;i<rnum.size();i++){
                rnum[i] = interval(gen);
                srnum[i] = rnum[i];
            }
            std::sort(srnum.begin(), srnum.end());
            // lim = srnum[int(diff_core_sizes->at(kindex) * (prob - last_prob))];
            lim = srnum[int(diff_core_sizes->at(kindex) * prob - current_sample_num->at(kindex))];

            int count = 0;
            for(unsigned int node=0;node<g->EdgeList.size();node++)
                for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                    int nbr = g->EdgeList[node]->at(j);
                    if(nbr > node){
                        int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
                        if(corevalue >= k && !sampled_matrix[node]->at(j)){
                            if(rnum[count] < lim){
                                if(!g->ProbeEdgeList[node]->at(j)){
                                    probe_count++;
                                    g->ProbeEdgeList[node]->at(j) = true;
                                    unsigned int l=0;
                                    for(;l<g->EdgeList[nbr]->size();l++)
                                        if(g->EdgeList[nbr]->at(l) == node) break;
                                    g->ProbeEdgeList[nbr]->at(l) = true;
                                }
                                sampled_matrix[node]->at(j) = true;
                                current_sample_num->at(kindex)++;
                                unsigned int l=0;
                                for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
                                sampled_matrix[nbr]->at(l) = true;
                                if(g->PrimeEdgeList[node]->at(j)){
                                    Rsubgraph[node]->push_back(nbr);
                                    Rsubgraph[nbr]->push_back(node);
                                }
                            }
                            count++;
                        }
                    }
                }
        }
        else{
            // std::cout<<"shell_sampling ********"<<std::endl;
            unsigned int remained_shell_edge = diff_core_sizes->at(kindex) - diff_core_sizes->at(pre_kindex);
            rnum.resize(remained_shell_edge);
            srnum.resize(remained_shell_edge);

            for(unsigned int i=0;i<rnum.size();i++){
                rnum[i] = interval(gen);
                srnum[i] = rnum[i];
            }
            std::sort(srnum.begin(), srnum.end());

            lim = srnum[int(remained_shell_edge * prob)];

            int count = 0;
            for(unsigned int node=0;node<g->EdgeList.size();node++)
                for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
                    int nbr = g->EdgeList[node]->at(j);
                    if(nbr > node){
                        int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
                        bool core_determ = (corevalue < pre_k && corevalue >= k);
                        if(core_determ && !sampled_matrix[node]->at(j)){
                            if(rnum[count] < lim){
                                if(!g->ProbeEdgeList[node]->at(j)){
                                    probe_count++;
                                    g->ProbeEdgeList[node]->at(j) = true;
                                    unsigned int l=0;
                                    for(;l<g->EdgeList[nbr]->size();l++)
                                        if(g->EdgeList[nbr]->at(l) == node) break;
                                    g->ProbeEdgeList[nbr]->at(l) = true;
                                }
                                sampled_matrix[node]->at(j) = true;
                                current_sample_num->at(kindex)++;
                                unsigned int l=0;
                                for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
                                sampled_matrix[nbr]->at(l) = true;
                                if(g->PrimeEdgeList[node]->at(j)){
                                    Rsubgraph[node]->push_back(nbr);
                                    Rsubgraph[nbr]->push_back(node);
                                }
                            }
                            count++;
                        }
                    }
                }
        }
        return probe_count;
    }

    // int edge_sampling_count_counstant_stack(double prob, double last_prob, int kindex, std::vector<int>* current_sample_num, std::vector<int>* diff_kvalues, std::vector<int>* diff_shell_sizes, 
    //                                                             std::vector<int>* diff_core_sizes, bool core_sample){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     int probe_count = 0;
    //     std::vector<double> rnum, srnum;
    //     double lim;

    //     int k = diff_kvalues->at(kindex);

    //     if(core_sample){
    //         int remained_core_edge = diff_core_sizes->at(kindex) - current_sample_num->at(kindex);
    //         rnum.resize(remained_core_edge);
    //         srnum.resize(remained_core_edge);

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }

    //         std::sort(srnum.begin(), srnum.end());
    //         lim = srnum[int(remained_core_edge * (prob - last_prob))];

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     if(corevalue >= k && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 current_sample_num->at(kindex)++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                             if(g->PrimeEdgeList[node]->at(j)){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
    //     }
    //     else{
    //         unsigned int remained_shell_edge = diff_shell_sizes->at(kindex);
    //         rnum.resize(remained_shell_edge);
    //         srnum.resize(remained_shell_edge);

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }
    //         std::sort(srnum.begin(), srnum.end());

    //         lim = srnum[int(remained_shell_edge * prob)];

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     bool core_determ = (corevalue < diff_kvalues->at(kindex + 1) && corevalue >= k);
    //                     if(core_determ && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                             if(g->PrimeEdgeList[node]->at(j)){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
    //     }
    //     return probe_count;
    // }

    // std::pair<int, double> edge_sampling_count_constant_stack(int pnum, int kindex, std::vector<int>* current_sample_num, double shell_check_prob,
    //                                                         std::vector<int>* diff_kvalues, std::vector<int>* diff_shell_sizes, std::vector<int>* diff_core_sizes){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     int probe_count = 0;
    //     double sampling_prob = 0.0; // the returning sampling probability
        
    //     std::vector<double> rnum, srnum;
    //     double lim;

    //     int count = 0;

    //     int k = diff_kvalues->at(kindex);

    //     int shell_edge_num = 0;
    //     double shell_sampled_prob = 0.0;
    //     if(kindex < diff_core_sizes->size() - 1) shell_edge_num = diff_shell_sizes->at(kindex);
    //     if(shell_edge_num > 0) shell_sampled_prob = (current_sample_num->at(kindex) * 1.0) / shell_edge_num;
    //     double core_sampled_prob = 0.0;
    //     if(kindex < current_sample_num->size() - 1) core_sampled_prob = (current_sample_num->at(kindex + 1) * 1.0) / diff_core_sizes->at(kindex + 1);

        
    //     //std::cout<<std::endl<<std::endl;
    //     //std::cout<<"kindex:"<<kindex<<std::endl;
    //     //std::cout<<"k:"<<k<<std::endl;
    //     //std::cout<<"shell_edge_num:"<<shell_edge_num<<std::endl;
    //     //std::cout<<"shell_sampled_prob:"<<shell_sampled_prob<<std::endl;
    //     //std::cout<<"core_sample_prob:"<<core_sampled_prob<<std::endl;
        
    //     if(kindex < diff_core_sizes->size()-1 && shell_sampled_prob < core_sampled_prob){
    //         // std::cout<<"start shell sampling ***********"<<std::endl;

    //         bool core_sample_flag = false;

    //         unsigned int remained_shell_edge = shell_edge_num - current_sample_num->at(kindex);
    //         rnum.resize(remained_shell_edge);
    //         srnum.resize(remained_shell_edge);

    //         // std::cout<<"remained_shell_edge:"<<remained_shell_edge<<std::endl;

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }
    //         std::sort(srnum.begin(), srnum.end());

    //         auto shell_plan_sample_num = (int)ceil(diff_shell_sizes->at(kindex) * core_sampled_prob - 0.000000001);
    //         if(shell_plan_sample_num - current_sample_num->at(kindex) > pnum){
    //             lim = srnum[pnum];
    //             // std::cout<<"shell_real_sample_num:"<<pnum<<"   PNUM"<<std::endl;
    //         }
    //         else {
    //             if(shell_plan_sample_num - current_sample_num->at(kindex) < srnum.size())
    //                 lim = srnum[shell_plan_sample_num - current_sample_num->at(kindex)];
    //             else lim = 1.0;
    //             // std::cout<<"shell_real_sample_num:"<<(shell_plan_sample_num - current_sample_num->at(kindex))<<"   NOT PNUM"<<std::endl;
    //             core_sample_flag = true;
    //         }

    //         // std::cout<<"shell_plan_sample_num:"<<shell_plan_sample_num<<std::endl;

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     bool core_determ = (corevalue < diff_kvalues->at(kindex + 1) && corevalue >= k);
    //                     if(core_determ && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
            
    //         if(shell_plan_sample_num - current_sample_num->at(kindex) >= pnum)
    //             current_sample_num->at(kindex) += pnum;
    //         else 
    //             current_sample_num->at(kindex) += (shell_plan_sample_num - current_sample_num->at(kindex));
    //         shell_sampled_prob = (current_sample_num->at(kindex) * 1.0) / shell_edge_num;
    //         // std::cout<<"updated_shell_sampled_prob:"<<shell_sampled_prob<<std::endl;

    //         double resample_prob = shell_sampled_prob / core_sampled_prob;
    //         // std::cout<<"resample_prob:"<<resample_prob<<std::endl;

    //         Rsubgraph_clear();
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     bool core_determ = (corevalue < diff_kvalues->at(kindex + 1) && corevalue >= k);
    //                     if(core_determ){
    //                         if(sampled_matrix[node]->at(j) && g->PrimeEdgeList[node]->at(j)){
    //                             Rsubgraph[node]->push_back(nbr);
    //                             Rsubgraph[nbr]->push_back(node);
    //                         }
    //                     }
    //                     else if(corevalue >= diff_kvalues->at(kindex + 1)){
    //                         if(sampled_matrix[node]->at(j) && g->PrimeEdgeList[node]->at(j)){
    //                             if(interval(gen) < resample_prob){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         if(core_sample_flag){
    //             current_sample_num->at(kindex) += current_sample_num->at(kindex + 1);
    //             current_sample_num->pop_back();
    //             sampling_prob = (current_sample_num->at(kindex) * 1.0) / diff_core_sizes->at(kindex);
    //         }
    //         else sampling_prob = (current_sample_num->at(kindex) * 1.0) / diff_shell_sizes->at(kindex);
    //         // std::cout<<"core_sample_flag:"<<core_sample_flag<<std::endl;
    //     }
    //     else{
    //         // std::cout<<"start core sampling **********"<<std::endl;
    //         int remained_core_edge = diff_core_sizes->at(kindex) - current_sample_num->at(kindex);
    //         // std::cout<<"remained_core_edge:"<<remained_core_edge<<std::endl;

    //         rnum.resize(remained_core_edge);
    //         srnum.resize(remained_core_edge);

    //         for(unsigned int i=0;i<rnum.size();i++){
    //             rnum[i] = interval(gen);
    //             srnum[i] = rnum[i];
    //         }
    //         std::sort(srnum.begin(), srnum.end());
            
    //         if(remained_core_edge <= pnum) lim = 1.0;
    //         else lim = srnum[pnum];

    //         int count = 0;
    //         for(unsigned int node=0;node<g->EdgeList.size();node++)
    //             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //                 int nbr = g->EdgeList[node]->at(j);
    //                 if(nbr > node){
    //                     int corevalue = std::min(g->CoreNumber[nbr], g->CoreNumber[node]);
    //                     if(corevalue >= k && !sampled_matrix[node]->at(j)){
    //                         if(rnum[count] < lim){
    //                             if(!g->ProbeEdgeList[node]->at(j)){
    //                                 probe_count++;
    //                                 current_sample_num->at(kindex)++;
    //                                 g->ProbeEdgeList[node]->at(j) = true;
    //                                 unsigned int l=0;
    //                                 for(;l<g->EdgeList[nbr]->size();l++)
    //                                     if(g->EdgeList[nbr]->at(l) == node) break;
    //                                 g->ProbeEdgeList[nbr]->at(l) = true;
    //                             }
    //                             sampled_matrix[node]->at(j) = true;
    //                             unsigned int l=0;
    //                             for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
    //                             sampled_matrix[nbr]->at(l) = true;
    //                             if(g->PrimeEdgeList[node]->at(j)){
    //                                 Rsubgraph[node]->push_back(nbr);
    //                                 Rsubgraph[nbr]->push_back(node);
    //                             }
    //                         }
    //                         count++;
    //                     }
    //                 }
    //             }
    //         sampling_prob = (current_sample_num->at(kindex) * 1.0) / diff_core_sizes->at(kindex);
    //     }

    //     // std::cout<<"Rsubgraph:"<<std::endl;
    //     // for(unsigned int n=0;n<Rsubgraph.size();n++){
    //     //    std::cout<<"n"<<n<<": ";
    //     //    for(unsigned int i=0;i<Rsubgraph[n]->size();i++)
    //     //        std::cout<<Rsubgraph[n]->at(i)<<" ";
    //     //    std::cout<<std::endl;
    //     // }
    //     // std::cout<<std::endl;

    //     // std::cout<<"sampled_matrix:"<<std::endl;
    //     // for(unsigned int n=0;n<sampled_matrix.size();n++){
    //     //    std::cout<<"n"<<n<<": ";
    //     //    for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
    //     //        std::cout<<sampled_matrix[n]->at(i)<<" ";
    //     //    std::cout<<std::endl;
    //     // }

    //     // std::cout<<"current_sample_num:";
    //     // for(unsigned int i=0;i<current_sample_num->size();i++) std::cout<<current_sample_num->at(i)<<" "; std::cout<<std::endl;
    //     return {probe_count, sampling_prob};
    // }

    // int edge_sampling_count_constant(int pnum, int valid_edge_num){
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     int probe_count = 0;

    //     std::vector<double> rnum, srnum;
    //     double lim;

    //     int k = (int)ceil(g->PrimeDS - 0.000000001);

    //     int count = 0;

    //     rnum.resize(valid_edge_num);
    //     srnum.resize(valid_edge_num);

    //     for(unsigned int i=0;i<rnum.size();i++){
    //         rnum[i] = interval(gen);
    //         srnum[i] = rnum[i];
    //     }

    //     // std::cout<<"rnum:";
    //     // for(unsigned int i=0;i<rnum.size();i++) std::cout<<rnum[i]<<" "; std::cout<<std::endl;

    //     std::sort(srnum.begin(), srnum.end());
        
    //     // std::cout<<"srnum:";
    //     // for(unsigned int i=0;i<srnum.size();i++) std::cout<<srnum[i]<<" "; std::cout<<std::endl;

    //     lim = srnum[pnum];
    //     // std::cout<<"lim:"<<lim<<std::endl;

    //     for(unsigned int i=0;i<g->EdgeList.size();i++){
    //         for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
    //             int nbr = g->EdgeList[i]->at(j);
    //             if(nbr > i && g->ValidEdgeList[i]->at(j) && !sampled_matrix[i]->at(j)){
    //                 if(rnum[count] < lim){
    //                     if(g->PrimeEdgeList[i]->at(j)){
    //                         Rsubgraph[i]->push_back(nbr);
    //                         Rsubgraph[nbr]->push_back(i);
    //                     }
    //                     sampled_matrix[i]->at(j) = true;
    //                     unsigned int l=0;
    //                     for(;l<g->EdgeList[nbr]->size();l++)
    //                         if(g->EdgeList[nbr]->at(l) == i) break;
    //                     sampled_matrix[nbr]->at(l) = true;
    //                     if(!g->ProbeEdgeList[i]->at(j)){
    //                         probe_count++;
    //                         g->ProbeEdgeList[i]->at(j) = true;
    //                         unsigned int l=0;
    //                         for(;l<g->EdgeList[nbr]->size();l++)
    //                             if(g->EdgeList[nbr]->at(l) == i) break;
    //                         g->ProbeEdgeList[nbr]->at(l) = true;
    //                     }
    //                 }
    //                 count++;
    //             }
    //         }
    //     }

    //     // std::cout<<"Rsubgraph:"<<std::endl;
    //     // for(unsigned int n=0;n<Rsubgraph.size();n++){
    //         // std::cout<<"n"<<n<<": ";
    //         // for(unsigned int i=0;i<Rsubgraph[n]->size();i++)
    //             // std::cout<<Rsubgraph[n]->at(i)<<" ";
    //         // std::cout<<std::endl;
    //     // }
    //     // std::cout<<std::endl;

    //     // std::cout<<"sampled_matrix:"<<std::endl;
    //     // for(unsigned int n=0;n<sampled_matrix.size();n++){
    //         // std::cout<<"n"<<n<<": ";
    //         // for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
    //             // std::cout<<sampled_matrix[n]->at(i)<<" ";
    //         // std::cout<<std::endl;
    //     // }
    //     return probe_count;
    // }

    // std::pair<int, bool> edge_sampling_count_constant(double lb, double sample_ratio, double epsilon, double constant, std::vector<int>* remained_core_edge_num){
    //     int probe_count = 0;

    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     for(unsigned int i=0;i<g->EdgeList.size();i++)
    //         for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
    //             int nbr = g->EdgeList[i]->at(j);
    //             if(nbr > i && g->ValidEdgeList[i]->at(j)){
    //                 if(interval(gen) < sample_ratio){
    //                     if(g->PrimeEdgeList[i]->at(j)){
    //                         Rsubgraph[i]->push_back(nbr);
    //                         Rsubgraph[nbr]->push_back(i);
    //                     }
    //                     if(!g->ProbeEdgeList[i]->at(j)){
    //                         g->ProbeEdgeList[i]->at(j) = true;
    //                         probe_count++;
    //                         unsigned int l=0;
    //                         for(;l<g->EdgeList[nbr]->size();l++)
    //                             if(g->EdgeList[nbr]->at(l) == i) break;
    //                         g->ProbeEdgeList[nbr]->at(l) = true;

    //                         int edge_core = std::min(g->CoreNumber[i], g->CoreNumber[nbr]);
    //                         for(unsigned int k=0;k<=edge_core;k++)
    //                             remained_core_edge_num->at(k) -= 1;
    //                     }
    //                 }
    //             }
    //         }

    //     auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //     delete Rsubgraph_dsd.second;
    //     double expected_dsd = Rsubgraph_dsd.first / sample_ratio;

    //     std::cout<<"expected_dsd:"<<expected_dsd<<std::endl;

    //     Rsubgraph_clear();

    //     std::cout<<"expected_dsd*epsilon:"<<expected_dsd * epsilon<<std::endl;
    //     std::cout<<"constant:"<<constant<<std::endl;
    //     std::cout<<"constant*sqrt(1/sample_ratio - 1):"<<constant * sqrt(1/sample_ratio - 1)<<std::endl;

    //     double ub = expected_dsd * epsilon + constant * sqrt(1/sample_ratio - 1);

    //     std::cout<<"lb:"<<lb<<std::endl;
    //     std::cout<<"ub:"<<ub<<std::endl;

    //     return {probe_count, lb >= ub};
    // }

    // std::pair<double, double> edge_sampling_count(double lb){
    //     double delta = 0.05;

    //     std::vector<double> rnum((unsigned int)g->ValidEdgeNum, 0.0);
    //     std::vector<double> srnum((unsigned int)g->ValidEdgeNum, 0.0);

    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::uniform_real_distribution<> interval(0.0, 1.0);

    //     for(int i = 0; i< g->ValidEdgeNum;i++){
    //         rnum[i] = interval(gen);
    //         srnum[i] = rnum[i];
    //     }

    //     std::sort(srnum.begin(), srnum.end());

    //     int sample_num = int(sample_ratio * g->ValidEdgeNum);
    //     double sample_prob = sample_ratio;
    //     //int sample_num = int(sample_ratio * g->ValidEdgeNum / (g->PrimeDS - lb));
    //     //double sample_prob = sample_ratio / (g->PrimeDS - lb);

    //     double lim = srnum[sample_num];

    //     int count = 0;

    //     double sample_probe_count = 0.0;

    //     for(unsigned int node=0;node<g->EdgeList.size();node++)
    //         for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
    //             int nbr = g->EdgeList[node]->at(j);
    //             if(nbr > node && g->ValidEdgeList[node]->at(j)){
    //                 if(rnum[count] < lim){
    //                     if(!g->ProbeEdgeList[node]->at(j)){
    //                         sample_probe_count++;
    //                         //g->ProbeEdgeList[node]->at(j) = true;
    //                         unsigned int l=0;
    //                         for(;l<g->EdgeList[nbr]->size();l++)
    //                             if(g->EdgeList[nbr]->at(l) == node) break;
    //                         //g->ProbeEdgeList[nbr]->at(l) = true;
    //                     }
    //                     if(g->PrimeEdgeList[node]->at(j)) {
    //                         Rsubgraph[node]->push_back(nbr);
    //                         Rsubgraph[(unsigned int)nbr]->push_back(node);
    //                     }
    //                 }
    //                 count++;
    //             }
    //         }

    //     auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //     delete Rsubgraph_dsd.second;
    //     double expected_dsd = Rsubgraph_dsd.first / sample_prob;

    //     double eps = 1 - (lb * (1 - delta) / expected_dsd);
    //     return {sample_probe_count, eps};
    // }

    // unsigned int verif(double lb){
    //     int kvalue = (int)ceil(lb-0.000000001);

    //     std::cout<<"kvalue:"<<kvalue<<std::endl;

    //     g->set_valid_edge(kvalue);

    //     double avg_verfication_probe_count = 0.0, min_eps = 1.0;

    //     for(unsigned int i=0;i<1;i++) {
    //         auto edge_sampling_res = edge_sampling_count(lb);

    //         if(edge_sampling_res.second < min_eps) min_eps = edge_sampling_res.second;
    //         avg_verfication_probe_count += edge_sampling_res.first;

    //         std::cout << "verification_probe_count:" << edge_sampling_res.first << std::endl;
    //         std::cout << "eps:" << edge_sampling_res.second << std::endl;
    //     }
    //     avg_verfication_probe_count /= 1;

    //     std::cout<<"avg_verification_probe_count:"<<avg_verfication_probe_count<<std::endl;
    //     std::cout<<"min_eps:"<<min_eps<<std::endl;
    // }

    // void verif_cost_gradient(double ratio){
    //     g->print_core_edge_num();

    //     auto probe_prime_densities_norm = new std::vector<double>();
    //     auto probe_prime_densities = new std::vector<double>();
    //     probe_prime_densities_norm->push_back(0);
    //     probe_prime_densities->push_back(0);
    //     int pnum = int(ratio * g->EdgeNum);

    //     std::cout<<"pnum:"<<pnum<<std::endl;

    //     int current_edge = 0;
    //     double gradient = 0.0;

    //     while(current_edge + pnum <=g->EdgeNum){
    //         exupdate_sample_matrix(current_edge, pnum);

    //         auto probed_prime = exact_dsd(&ProbedPrimeG);
    //         delete probed_prime.second;

    //         current_edge += pnum;

    //         probe_prime_densities_norm->push_back((probed_prime.first / g->PrimeDS));
    //         probe_prime_densities->push_back(probed_prime.first);

    //         std::cout<<"probe_prime_density_ratio:"<<probed_prime.first / g->PrimeDS<<std::endl;
    //         std::cout<<"probe_prime_density:"<<probed_prime.first<<std::endl;

    //         if(probe_prime_densities_norm->size() > 3){
    //             double current_prime_density = probe_prime_densities->at(probe_prime_densities->size()-1);
    //             double last_prime_density = probe_prime_densities->at(probe_prime_densities->size()-3);

    //             std::cout<<"current_prime_density:"<<current_prime_density<<std::endl;
    //             std::cout<<"last_prime_density:"<<last_prime_density<<std::endl;

    //             gradient = (current_prime_density - last_prime_density) / (pnum * 2);
    //             std::cout<<"gradient:"<<gradient<<std::endl;

    //             double diff = ceil(current_prime_density) - current_prime_density;
    //             std::cout<<"diff:"<<diff<<std::endl;

    //             double required_probe_count = diff / gradient;
    //             std::cout<<"required_probe_count:"<<required_probe_count<<std::endl;

    //             int kvalue = (int)ceil(current_prime_density-0.000000001);
    //             std::cout<<"kvalue:"<<kvalue<<std::endl;

    //             int core_edge_diff = g->CoreEdgeNum[kvalue] - g->CoreEdgeNum[kvalue + 1];
    //             std::cout<<"core_edge_diff:"<<core_edge_diff<<std::endl;

    //             double sample_reduce = core_edge_diff * ratio;
    //             std::cout<<"sample_reduce:"<<sample_reduce<<std::endl;

    //             if(required_probe_count > sample_reduce)
    //                 std::cout<<"required_probe_count >>>>>> sample_reduce"<<std::endl;
    //             else
    //                 std::cout<<"required_probe_count <<<<<< sample_reduce"<<std::endl;
    //         }
    //     }
    // }

    // void residual_constant(){
    //     std::cout<<"g->PrimeDS:"<<g->PrimeDS<<std::endl;

    //     std::vector<double> ps = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.99};
    //     std::vector<double> constants;
    //     for(double p : ps){
    //         double max_residual = 0.0;
    //         for(unsigned int i=0;i<1000;i++){
    //             double lb_diff = global_edge_sampling(p);
    //             if(lb_diff > max_residual) max_residual = lb_diff;
    //         }

    //         std::cout<<"p:"<<p<<" max_residual:"<<max_residual<<std::endl;
    //         // double constant = max_residual * sqrt(p);
    //         double constant = max_residual / (sqrt(1/p) - 1);
    //         constants.push_back(constant);
    //     }
    //     for(unsigned int i=0;i<ps.size();i++)
    //         std::cout<<"p:"<<ps[i]<<" constant:"<<constants[i]<<std::endl;
    // }

    std::pair<std::pair<double, double>, int> verif_cost_sample_induced_lb(double epsilon, double constant, double alpha){
    //std::pair<double, double> verif_cost_sample_induced_lb(double epsilon, double constant, double alpha){
        double sampling_prob = alpha;
        double pre_sampling_prob = 0.0;
        double beta = 1 - alpha;

        int number_of_iterations = 0;

        auto current_sample_num = new std::vector<int>(1, 0);
        
        double lb = 0;
        int probe_count = 0;
        double dsd_est = 0;

        while(true){
            number_of_iterations++;
            int res = edge_sampling_count_constant(sampling_prob, current_sample_num);
            probe_count += res;

            auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
            auto induced_res = get_sample_DSD_induced(Rsubgraph_dsd.second);
            delete Rsubgraph_dsd.second;
            probe_count += induced_res.second;

            if(induced_res.first > lb) lb = induced_res.first;

            dsd_est = Rsubgraph_dsd.first / sampling_prob;

            double ub = dsd_est * epsilon + constant * sqrt(1/sampling_prob - 1);
            if(lb >= ub) break;
            pre_sampling_prob = sampling_prob;
            sampling_prob += alpha * beta;
            beta *= (1-alpha);
        }
        double pratio = probe_count * 1.0 / g->EdgeNum;
        double precision = lb / g->PrimeDS;
        Rsubgraph_clear();
        sample_matrix_clear();
        g->clearProbeEdgeList();
        
        return {{pratio, precision}, number_of_iterations};
    }

    std::pair<std::pair<double, double>, int> verif_cost_sample_induced_lb_stacked_prob(double epsilon, double constant, double alpha){
        // std::cout<<"g->PrimeDS:"<<g->PrimeDS<<std::endl;

        auto diff_shell_sizes = new std::vector<int>();
        auto diff_kvalues = new std::vector<int>();
        auto diff_core_sizes = new std::vector<int>();

        double sampling_prob = alpha;
        double pre_sampling_prob = 0.0;
        double beta = 1 - alpha;
        bool core_sampling = true;

        diff_kvalues->push_back(0);
        for(unsigned int i = 1;i <= g->CoreEdgeNum.size() - 1;i++){
            while(g->CoreEdgeNum[i] == g->CoreEdgeNum[i - 1]) i++;
            if(i >=g->CoreEdgeNum.size()) break;
            diff_kvalues->push_back(i);
        }

        for(unsigned int i=0;i<diff_kvalues->size();i++)
            diff_shell_sizes->push_back(g->CoreEdgeNum[diff_kvalues->at(i)]);
        for(unsigned int i=0;i<diff_shell_sizes->size()-1;i++)
            diff_shell_sizes->at(i) -= diff_shell_sizes->at(i+1);
        for(unsigned int i=0;i<diff_kvalues->size();i++)
            diff_core_sizes->push_back(g->CoreEdgeNum[diff_kvalues->at(i)]);

        // for(unsigned int i=0;i<diff_kvalues->size();i++)
        //     std::cout<<"("<<diff_kvalues->at(i)<<", "<<diff_shell_sizes->at(i)<<", "<<diff_core_sizes->at(i)<<")"<<std::endl;
        // std::cout<<"(diff_kvalues, diff_shell_sizes, diff_core_sizes)"<<std::endl;

        int diff_kvalue_index = diff_kvalues->size() - 1;
        int pre_kvalue_index = diff_kvalues->size() - 1;

        double lb = 0.0;
        int probe_count = 0;

        auto current_sample_num = new std::vector<int>(diff_shell_sizes->size(), 0);
        double dsd_est = 0;

        int number_of_iterations = 0;

        while(true){
            // std::cout<<std::endl<<std::endl<<std::endl;
            // std::cout<<"kvalue_index:"<<diff_kvalue_index<<std::endl;
            // std::cout<<"kvalue:"<<diff_kvalues->at(diff_kvalue_index)<<std::endl;
            // std::cout<<"sampling_prob:"<<sampling_prob<<std::endl;
            // std::cout<<"pre_kvalue_index:"<<pre_kvalue_index<<std::endl;
            // std::cout<<"pre_kvalue:"<<diff_kvalues->at(pre_kvalue_index)<<std::endl;
            // std::cout<<"pre_sampling_prob:"<<pre_sampling_prob<<std::endl;

            number_of_iterations++;

            if(core_sampling){
                int i = current_sample_num->size()-1;
                while(i > diff_kvalue_index) {
                    i--;
                    current_sample_num->at(i) += current_sample_num->at(i+1);
                }
                while(current_sample_num->size() > diff_kvalue_index + 1) current_sample_num->pop_back();
                // std::cout<<"current_sample_num:";
                // for(unsigned int i=0;i<current_sample_num->size();i++)
                //     std::cout<<"("<<diff_kvalues->at(i)<<", "<<current_sample_num->at(i)<<") ";
                // std::cout<<std::endl;
            }

            int res = edge_sampling_count_constant_stack(sampling_prob, pre_sampling_prob, diff_kvalue_index, pre_kvalue_index, current_sample_num, diff_kvalues, diff_shell_sizes, diff_core_sizes, core_sampling);
            probe_count += res;
            // std::cout<<"step_probe_count:"<<res<<std::endl;

            // std::cout<<"current_sample_num:";
            // for(unsigned int i=0;i<current_sample_num->size();i++)
                // std::cout<<"("<<diff_kvalues->at(i)<<", "<<current_sample_num->at(i)<<") ";
            // std::cout<<std::endl;

            auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
            auto induced_res = get_sample_DSD_induced(Rsubgraph_dsd.second);
            delete Rsubgraph_dsd.second;
            probe_count += induced_res.second;

            pre_kvalue_index = diff_kvalue_index;

            // std::cout<<"updated_pre_kvalue_index:"<<pre_kvalue_index<<std::endl;
            // std::cout<<"updated_pre_kvalue:"<<diff_kvalues->at(pre_kvalue_index)<<std::endl;

            // std::cout<<"current_probe_count:"<<probe_count<<std::endl;

            // std::cout<<"dsd_est:"<<Rsubgraph_dsd.first<<std::endl;

            if(induced_res.first > lb) lb = induced_res.first;
            // std::cout<<"lb:"<<lb<<std::endl;

            dsd_est = Rsubgraph_dsd.first / sampling_prob;

            // std::cout<<"normalized_dsd_est:"<<dsd_est<<std::endl;

            double ub = dsd_est * epsilon + constant * sqrt(1/sampling_prob - 1);
            // std::cout<<"ub:"<<ub<<std::endl;

            // std::cout<<"Rsubgraph:"<<std::endl;
            // for(unsigned int n=0;n<Rsubgraph.size();n++){
            //     std::cout<<"n"<<n<<": ";
            //     for(unsigned int i=0;i<Rsubgraph[n]->size();i++)
            //         std::cout<<Rsubgraph[n]->at(i)<<" ";
            //     std::cout<<std::endl;
            // }
            // std::cout<<std::endl;

            // std::cout<<"sampled_matrix:"<<std::endl;
            // for(unsigned int n=0;n<sampled_matrix.size();n++){
            //     std::cout<<"n"<<n<<": ";
            //     for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
            //        std::cout<<sampled_matrix[n]->at(i)<<" ";
            //     std::cout<<std::endl;
            // }

            if(lb >= ub && lb > diff_kvalues->at(diff_kvalue_index) - 1){
                break;
            }

            if((dsd_est * epsilon) < diff_kvalues->at(diff_kvalue_index) - 1){
                core_sampling = false;
                // diff_kvalue_index--;

                int kvalue_gap = int(diff_kvalues->at(diff_kvalue_index) - 1 - dsd_est * epsilon) / 2;
                // std::cout<<"kvalue_gap:"<<kvalue_gap<<std::endl;
                if(kvalue_gap < 1) diff_kvalue_index--;
                else{
                    while(diff_kvalues->at(pre_kvalue_index) - diff_kvalues->at(diff_kvalue_index) < kvalue_gap){
                        diff_kvalue_index--;
                        // std::cout<<"pre_kvalue - cur_kvalue:"<<diff_kvalues->at(pre_kvalue_index) - diff_kvalues->at(diff_kvalue_index)<<std::endl;
                    }
                }
            }
            else{
                pre_sampling_prob = sampling_prob;
                sampling_prob += alpha * beta;
                beta *= (1-alpha);
                core_sampling = true;
            }
        }

        delete current_sample_num;
        delete diff_core_sizes;
        delete diff_kvalues;
        delete diff_shell_sizes;

        double pratio = probe_count * 1.0 / g->EdgeNum;
        double precision = lb / g->PrimeDS;
        Rsubgraph_clear();
        sample_matrix_clear();
        g->clearProbeEdgeList();

        return {{pratio, precision}, number_of_iterations};
    }

    // std::pair<double, double> verif_cost_sample_induced_lb_stacked(double gap, double epsilon, double constant, double expand=0.0){
    //     // g->print_core_edge_num();
    //     // std::cout<<"g->CoreEdgeNum_size:"<<g->CoreEdgeNum.size()<<std::endl;
    //     std::cout<<"g->PrimeDS:"<<g->PrimeDS<<std::endl;
        
    //     double dsd_time = 0.0;
    //     double sampling_time = 0.0;
    //     double coreeaxt_dsd_time = 0.0;

    //     auto diff_shell_sizes = new std::vector<int>();
    //     auto diff_kvalues = new std::vector<int>();
    //     auto diff_core_sizes = new std::vector<int>();
        
    //     diff_kvalues->push_back(0);
    //     for(unsigned int i = 1;i <= g->CoreEdgeNum.size() - 1;i++){
    //         while(g->CoreEdgeNum[i] == g->CoreEdgeNum[i - 1]) i++;
    //         if(i >=g->CoreEdgeNum.size()) break;
    //         diff_kvalues->push_back(i);
    //     }

    //     std::cout<<"diff_kvalues:";
    //     for(unsigned int i=0;i<diff_kvalues->size();i++) std::cout<<diff_kvalues->at(i)<<" "; std::cout<<std::endl;

    //     for(unsigned int i=0;i<diff_kvalues->size();i++)
    //         diff_shell_sizes->push_back(g->CoreEdgeNum[diff_kvalues->at(i)]);
    //     for(unsigned int i=0;i<diff_shell_sizes->size()-1;i++)
    //         diff_shell_sizes->at(i) -= diff_shell_sizes->at(i+1);
    //     for(unsigned int i=0;i<diff_kvalues->size();i++)
    //         diff_core_sizes->push_back(g->CoreEdgeNum[diff_kvalues->at(i)]);
        
    //     std::cout<<"diff_shell_sizes:";
    //     for(unsigned int i=0;i<diff_shell_sizes->size();i++) std::cout<<diff_shell_sizes->at(i)<<" "; std::cout<<std::endl;
    //     std::cout<<"diff_core_sizes:";
    //     for(unsigned int i=0;i<diff_core_sizes->size();i++) std::cout<<diff_core_sizes->at(i)<<" "; std::cout<<std::endl;

    //     // std::cout<<"diff_kvalues_count:"<<diff_kvalues->size()<<std::endl;
    //     // std::cout<<"diff_shell_count:"<<diff_shell_sizes->size()<<std::endl;
    //     // std::cout<<"diff_core_count:"<<diff_core_sizes->size()<<std::endl;

    //     int diff_kvalue_index = diff_kvalues->size() - 1;
    //     int pre_kvalue_index = diff_kvalue_index;
    //     int pnum = int(gap * g->EdgeNum);
    //     double lb = 0.0;
    //     int probe_count = 0;

    //     auto current_sample_num = new std::vector<int>(diff_shell_sizes->size(), 0);

    //     double dsd_est = 0;

    //     auto start = std::chrono::steady_clock::now();

    //     // std::string eps_str = std::to_string(epsilon).substr(0, 3);
    //     // std::string rname = "daas_"+g->name+"_"+this->ranking_function+"_eps"+eps_str;

    //     while(true){
    //         std::cout<<std::endl<<std::endl<<std::endl;
    //         std::cout<<"kvalue_index:"<<diff_kvalue_index<<std::endl;
    //         std::cout<<"kvalue:"<<diff_kvalues->at(diff_kvalue_index)<<std::endl;
    //         std::cout<<"pre_kvalue_index:"<<pre_kvalue_index<<std::endl;
    //         std::cout<<"pre_kvalue:"<<diff_kvalues->at(pre_kvalue_index)<<std::endl;
            
    //         auto start_sampling = std::chrono::steady_clock::now();
    //         auto res = edge_sampling_count_constant_stack(pnum, diff_kvalue_index, pre_kvalue_index, current_sample_num, diff_kvalues, diff_shell_sizes, diff_core_sizes);
    //         auto end_sampling = std::chrono::steady_clock::now();
    //         sampling_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_sampling - start_sampling).count();

    //         probe_count += res.first;

    //         std::cout<<"step_probe_ratio:"<< (res.first * 1.0) / g->EdgeNum<<std::endl;

    //         if(this->pre_kindex_update){
    //             pre_kvalue_index = diff_kvalue_index;
    //             this->pre_kindex_update = false;
    //         }
            
    //         auto start_dsd = std::chrono::steady_clock::now();
    //         auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //         auto end_dsd = std::chrono::steady_clock::now();
    //         dsd_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_dsd - start_dsd).count();

    //         auto induced_res = get_sample_DSD_induced(Rsubgraph_dsd.second);
    //         delete Rsubgraph_dsd.second;
    //         probe_count += induced_res.second;

    //         if(induced_res.first > lb) lb = induced_res.first;

    //         // std::cout<<"dsd_est:"<<Rsubgraph_dsd.first<<std::endl;

    //         dsd_est = Rsubgraph_dsd.first / res.second;

    //         /* std::cout<<"normalized_dsd_est:"<<dsd_est<<std::endl; */

    //         double ub;
    //         if(res.second >= 0.99999999) ub = dsd_est * epsilon;
    //         else ub = dsd_est * epsilon + constant * sqrt(1/res.second - 1);
            
    //         // std::cout<<"sampling_prob:"<<res.second<<std::endl;
    //         // std::cout<<"kvalue:"<<diff_kvalues->at(diff_kvalue_index)<<std::endl;
    //         // std::cout<<"ub:"<<ub<<std::endl;
    //         // std::cout<<"lb:"<<lb<<std::endl;
    //         // std::cout<<"dsd_est:"<<dsd_est<<std::endl<<std::endl;

    //         if(lb >= ub && lb > diff_kvalues->at(diff_kvalue_index) - 1) {
    //             // std::cout<<"terminate_kvalue:"<<diff_kvalues->at(diff_kvalue_index)<<std::endl;
    //             // std::cout<<"sampling_prob:"<<res.second<<std::endl;
    //             break;
    //         }

    //         // std::cout<<"kvalue_gap:"<< (dsd_est * epsilon - diff_kvalues->at(diff_kvalue_index) + 1)<<std::endl;
            
    //         if((dsd_est * epsilon) < diff_kvalues->at(diff_kvalue_index) - 1){
    //             int kvalue_gap = int(diff_kvalues->at(diff_kvalue_index) - 1 - dsd_est * epsilon) / 2;
    //             while(diff_kvalues->at(pre_kvalue_index) - diff_kvalues->at(diff_kvalue_index) < kvalue_gap)
    //                 diff_kvalue_index--;
    //         }
    //         if(expand >= 1.0) pnum = int(ceil(pnum * expand)+0.000000001);
    //     }

    //     auto end = std::chrono::steady_clock::now();
    //     double stacked_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    //     // std::cout<<"total_stack_time:"<< (stacked_time / 1000000000)<<"s"<<std::endl;
    //     // std::cout<<"total_dsd_time:"<<(dsd_time/1000000000)<<"s"<<std::endl;
    //     // std::cout<<"total_sampling_time:"<<(sampling_time/1000000000)<<"s"<<std::endl;
    //     // std::cout<<"total_coreexact_dsd_time:"<<coreeaxt_dsd_time<<"s"<<std::endl;

    //     delete current_sample_num;
    //     delete diff_core_sizes;
    //     delete diff_kvalues;
    //     delete diff_shell_sizes;

    //     double pratio = probe_count * 1.0 / g->EdgeNum;
    //     double precision = lb / g->PrimeDS;
    //     Rsubgraph_clear();
    //     sample_matrix_clear();
    //     g->clearProbeEdgeList();

    //     return {pratio, precision};
    // }

    // std::pair<double, double> verif_cost_sample_induced_lb(double gap, double epsilon, double constant){
    //     g->print_core_edge_num();

    //     int kvalue = (int)ceil(g->PrimeDS - 0.000000001);
    //     std::cout<<"kvalue:"<<kvalue<<std::endl;

    //     int valid_edge_num = g->CoreEdgeNum[kvalue];
    //     g->set_valid_edge(kvalue);
    //     int pnum = int(gap * valid_edge_num);

    //     double lb = 0.0;

    //     int probe_count = 0;
    //     int sample_num = 0;

    //     while(sample_num < g->CoreEdgeNum[kvalue]){
    //         sample_num += pnum;

    //         double sample_prob = (sample_num * 1.0) / g->CoreEdgeNum[kvalue];
            
    //         int res = edge_sampling_count_constant(pnum, valid_edge_num);
    //         valid_edge_num -= pnum;
    //         probe_count += res;

    //         auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);
    //         auto induced_res = get_sample_DSD_induced(Rsubgraph_dsd.second);
    //         delete Rsubgraph_dsd.second;
    //         probe_count += induced_res.second;
    //         if(induced_res.first > lb) lb = induced_res.first;

    //         // auto Rsubgraph_global_dsd = get_Rsubgraph_induced();
    //         // probe_count += Rsubgraph_global_dsd.second;
    //         // if(Rsubgraph_global_dsd.first > lb) lb = Rsubgraph_global_dsd.first;

    //         double ub = (Rsubgraph_dsd.first / sample_prob) * epsilon + constant * sqrt(1/sample_prob - 1);
    //         if(lb >= ub && lb > kvalue - 1) {
    //             std::cout<<"lb:"<<lb<<std::endl;
    //             std::cout<<"ub:"<<ub<<std::endl;
    //             std::cout<<"sample_prob:"<<sample_prob<<std::endl;
    //             break;
    //         }
    //     }

    //     double pratio = probe_count * 1.0 / g->EdgeNum;
    //     double precision = lb / g->PrimeDS;
    //     Rsubgraph_clear();
    //     sample_matrix_clear();
    //     g->clearProbeEdgeList();
    //     return {pratio, precision};
    // }

    // int verif_cost_constant_maintain_sample(double gap, double constant, double epsilon, double ratio){
    //     g->print_core_edge_num();
    //     std::cout<<"g->PrimeDS:"<<g->PrimeDS<<std::endl;

    //     int pnum = int(gap * g->EdgeNum);
    //     int current_edge = 0;
    //     int probe_count = 0;

    //     int deter_probe_count = 0;
    //     int sampl_probe_count = 0;

    //     double lb = 0.0;

    //     int kvalue = 0;

    //     int sample_num = 0;

    //     while(current_edge + pnum <= g->EdgeNum){
    //         int temp_count = exupdate_sample_matrix_constant(current_edge, pnum);
    //         probe_count += temp_count;
    //         deter_probe_count += temp_count;
    //         current_edge += pnum;
    //         std::cout<<"deter_probe_count:"<<deter_probe_count<<std::endl;
    //         std::cout<<"probe_count:"<<probe_count<<std::endl;
    //         std::cout<<"current_edge:"<<current_edge<<std::endl;
            
    //         auto probed_prime = exact_dsd(&ProbedPrimeG);
    //         delete probed_prime.second;
    //         lb = probed_prime.first;

    //         std::cout<<"lb:"<<lb<<std::endl;

    //         kvalue = (int)ceil(lb - 0.000000001);

    //         std::cout<<"kvalue:"<<kvalue<<std::endl;

    //         int valid_edge_num = g->CoreEdgeNum[kvalue];
    //         std::cout<<"valid_edge_num:"<<valid_edge_num<<std::endl;

    //         double required_sample_prob = 1 / (pow(lb / constant, 2) + 1);
    //         std::cout<<"required_sample_prob:"<<required_sample_prob<<std::endl;

    //         sample_num = int(required_sample_prob * valid_edge_num);
    //         std::cout<<"sample_num:"<<sample_num<<std::endl<<std::endl<<std::endl;

    //         if(sample_num < ratio * current_edge) break;
    //     }

    //     std::cout<<"START SAMPLING CHECK #########################"<<std::endl<<std::endl<<std::endl;

    //     bool first_sampling_round = true;

    //     std::vector<int>* sampled_dsd_nodes;

    //     kvalue = (int)ceil(lb - 0.000000001);

    //     int valid_edge_num = g->CoreEdgeNum[kvalue];

    //     g->set_valid_edge(kvalue);

    //     while(current_edge + pnum <= g->EdgeNum){
    //         double sampled_dsd = 0.0;
    //         double sample_prob = 0.0;

    //         if(first_sampling_round){
    //             int res = edge_sampling_count_constant(sample_num, valid_edge_num);
    //             valid_edge_num -= sample_num;
    //             std::cout<<"valid_edge_num:"<<valid_edge_num<<std::endl;
                
    //             first_sampling_round = false;

    //             probe_count += res;
    //             sampl_probe_count += res;

    //             auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);

    //             sampled_dsd = Rsubgraph_dsd.first;
    //             sample_prob = sample_num * 1.0 / g->CoreEdgeNum[kvalue];
    //             std::cout<<"sample_prob:"<<sample_prob<<std::endl;

    //             sampled_dsd_nodes = Rsubgraph_dsd.second;
    //         }
    //         else{
    //             auto res = edge_sampling_count_constant(pnum, valid_edge_num);
    //             valid_edge_num -= pnum;
    //             std::cout<<"valid_edge_num:"<<valid_edge_num<<std::endl;

    //             auto Rsubgraph_dsd = exact_dsd(&Rsubgraph);

    //             probe_count += res;
    //             sampl_probe_count += res;
    //             sampled_dsd = Rsubgraph_dsd.first;
    //             sample_num += pnum;
    //             sample_prob = sample_num * 1.0 / g->CoreEdgeNum[kvalue];
    //             std::cout<<"sample_prob:"<<sample_prob<<std::endl;

    //             sampled_dsd_nodes = Rsubgraph_dsd.second;
    //         }

    //         auto res = get_sample_DSD_induced(sampled_dsd_nodes);
    //         delete sampled_dsd_nodes;

    //         std::cout<<"induced_dsd:"<<res.first<<std::endl;
    //         std::cout<<"induced_probe_count:"<<res.second<<std::endl;
    //         probe_count += res.second;
    //         if(res.first > lb) lb = res.first;

    //         sampled_dsd = sampled_dsd / sample_prob;

    //         std::cout<<"sampled_dsd:"<<sampled_dsd<<std::endl;
    //         double ub = sampled_dsd * epsilon; //+ constant * sqrt(1/sample_prob - 1);
    //         std::cout<<"ub:"<<ub<<std::endl<<std::endl<<std::endl;
    //         if(lb >= ub) break;
    //     }
        
    //     std::cout<<"total_probe_count:"<<probe_count<<std::endl;
    //     std::cout<<"deter_probe_count:"<<deter_probe_count<<std::endl;
    //     std::cout<<"sampl_probe_count:"<<sampl_probe_count<<std::endl;
    //     std::cout<<"probe_ratio:"<<probe_count * 1.0 / g->EdgeNum<<std::endl;
    //     return probe_count;
    // }

    // int verif_cost_constant(double gap, double constant, double epsilon, double ratio){
    //     std::vector<int>* remained_core_edge_num = new std::vector<int>();
    //     for(unsigned int i=0;i<g->CoreEdgeNum.size();i++)
    //         remained_core_edge_num->push_back(g->CoreEdgeNum[i]);

    //     g->print_core_edge_num();
        
    //     std::cout<<"RemCoreENum:";
    //     for(unsigned int i=0;i<remained_core_edge_num->size();i++)
    //         std::cout<<remained_core_edge_num->at(i)<<" ";
    //     std::cout<<std::endl;

    //     std::cout<<"g->PrimeDS:"<<g->PrimeDS<<std::endl;
    //     // auto probe_prime_densities = new std::vector<double>();
    //     // probe_prime_densities->push_back(0);
    //     int pnum = int(gap * g->EdgeNum);
    //     int current_edge = 0;
    //     int probe_count = 0;

    //     int deter_probe_count = 0;
    //     int sampl_probe_count = 0;

    //     double lb;

    //     int kvalue = 0;
    //     int sample_num = 0;

    //     while(current_edge + pnum <= g->EdgeNum){
    //         int temp_count = exupdate_sample_matrix_constant(current_edge, pnum, remained_core_edge_num);
    //         probe_count += temp_count;
    //         deter_probe_count += temp_count;

    //         std::cout<<"RemCoreENum:";
    //         for(unsigned int i=0;i<remained_core_edge_num->size();i++)
    //             std::cout<<remained_core_edge_num->at(i)<<" ";
    //         std::cout<<std::endl;

    //         auto probed_prime = exact_dsd(&ProbedPrimeG);
    //         delete probed_prime.second;

    //         current_edge += pnum;

    //         // probe_prime_densities->push_back((probed_prime.first / g->PrimeDS));
    //         std::cout<<"current_prime_DSD:"<<probed_prime.first<<std::endl;
    //         std::cout<<"current_prime_DSD_approx:"<<probed_prime.first / g->PrimeDS<<std::endl;

    //         // if(probe_prime_densities->at(probe_prime_densities->size()-1) >= 0.99999999) break;

    //         lb = probed_prime.first;

    //         kvalue = (int)ceil(probed_prime.first - 0.000000001);
    //         std::cout<<"kvalue:"<<kvalue<<std::endl;

    //         // double valid_edge_num = g->CoreEdgeNum[kvalue];
    //         double valid_edge_num = remained_core_edge_num->at(kvalue);
    //         std::cout<<"valid_edge_num:"<<valid_edge_num<<std::endl;

    //         double sample_prob = 1 / (pow(probed_prime.first / constant, 2) + 1);
    //         std::cout<<"sample_prob:"<<sample_prob<<std::endl;

    //         sample_num = int(sample_prob * valid_edge_num);
    //         std::cout<<"sample_num:"<<sample_num<<std::endl;

    //         std::cout<<"current_edge:"<<current_edge<<std::endl;
    //         std::cout<<std::endl<<std::endl;

    //         if(sample_num < ratio * current_edge) break;
    //     }

    //     std::cout<<"START SAMPLING CHECK #########################"<<std::endl<<std::endl<<std::endl;

    //     while(current_edge + pnum <= g->EdgeNum){
    //         kvalue = (int)ceil(lb - 0.000000001);
    //         std::cout<<"kvalue:"<<kvalue<<std::endl;

    //         // int valid_edge_num = g->CoreEdgeNum[kvalue];
    //         int valid_edge_num = remained_core_edge_num->at(kvalue);
    //         std::cout<<"valid_edge_num:"<<valid_edge_num<<std::endl;

    //         //double sample_prob = 1 / (pow(lb / constant, 2) + 1);
    //         //std::cout<<"sample_prob:"<<sample_prob<<std::endl;

    //         //int sample_num = int(sample_prob * valid_edge_num);

    //         sample_num += pnum;
    //         std::cout<<"sample_num:"<<sample_num<<std::endl;

    //         double sample_prob = sample_num * 1.0 / valid_edge_num;
    //         if(sample_prob > 1) sample_prob = 1;
    //         std::cout<<"sample_prob:"<<sample_prob<<std::endl;

    //         g->set_valid_edge(kvalue);

    //         auto res = edge_sampling_count_constant(lb, sample_prob, epsilon, constant, remained_core_edge_num);
    //         std::cout<<"sampling_probe_count:"<<res.first<<std::endl;
    //         int temp_count = res.first;
    //         probe_count += temp_count;
    //         sampl_probe_count += temp_count;

    //         std::cout<<"RemCoreENum:";
    //         for(unsigned int i=0;i<remained_core_edge_num->size();i++)
    //             std::cout<<remained_core_edge_num->at(i)<<" ";
    //         std::cout<<std::endl;

    //         std::cout<<"current_prime_DSD_approx:"<<lb / g->PrimeDS<<std::endl;

    //         std::cout<<"satisfied:"<<res.second<<std::endl;

    //         std::cout<<std::endl<<std::endl;

    //         if(res.second) break;
    //         else{
    //             int temp_count = exupdate_sample_matrix_constant(current_edge, pnum, remained_core_edge_num);
    //             probe_count += temp_count;
    //             deter_probe_count += temp_count;

    //             current_edge += pnum;

    //             std::cout<<"RemCoreENum:";
    //             for(unsigned int i=0;i<remained_core_edge_num->size();i++)
    //                 std::cout<<remained_core_edge_num->at(i)<<" ";
    //             std::cout<<std::endl;

    //             lb = exact_dsd(&ProbedPrimeG).first;
    //             std::cout<<"current_prime_DSD:"<<lb<<std::endl;
    //         }
    //     }

    //     std::cout<<"total_probe_count:"<<probe_count<<std::endl;
    //     std::cout<<"deter_probe_count:"<<deter_probe_count<<std::endl;
    //     std::cout<<"sampl_probe_count:"<<sampl_probe_count<<std::endl;
    //     return probe_count;
    // }

    // std::vector<double>* verif_cost (double ratio){
    //     auto probe_prime_densities = new std::vector<double>();
    //     probe_prime_densities->push_back(0);
    //     int pnum = int(ratio * g->EdgeNum);
    //     int current_edge = 0;

    //     while(current_edge + pnum <=g->EdgeNum){
    //         exupdate_sample_matrix(current_edge, pnum);

    //         auto probed_prime = exact_dsd(&ProbedPrimeG);
    //         delete probed_prime.second;

    //         current_edge += pnum;

    //         std::cout<<"current_edge:"<<current_edge<<std::endl;

    //         std::cout<<"current_prime_DSD:"<<probed_prime.first<<std::endl;

    //         probe_prime_densities->push_back((probed_prime.first / g->PrimeDS));

    //         std::cout<<"current_prime_DSD_approx:"<<probed_prime.first / g->PrimeDS<<std::endl;

    //         if(probe_prime_densities->at(probe_prime_densities->size()-1) >= 0.99999999) break;

    //         verif(probed_prime.first);
    //     }
    //     return probe_prime_densities;
    // }

    // int exupdate_sample_matrix_constant(int current_edge, int pnum, std::vector<int>* remained_core_edge_num){
    //     int probe_count = 0;
    //     for(int i=current_edge;i<current_edge+pnum;i++){
    //         int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
    //         if(is_prime[i]){
    //             ProbedPrimeG[s]->push_back(t);
    //             ProbedPrimeG[t]->push_back(s);
    //         }
    //         unsigned int j=0;
    //         for(;j < g->EdgeList[s]->size();j++)
    //             if(g->EdgeList[s]->at(j) == t) break;
    //         if(!g->ProbeEdgeList[s]->at(j)){
    //             probe_count++;
    //             g->ProbeEdgeList[s]->at(j) = true;
    //             int edge_core = std::min(g->CoreNumber[s], g->CoreNumber[t]);
    //             for(unsigned int k=0;k <= edge_core;k++) remained_core_edge_num->at(k) -= 1;
    //         }
    //         j=0;
    //         for(;j < g->EdgeList[t]->size();j++)
    //             if(g->EdgeList[t]->at(j) == s) break;
    //         g->ProbeEdgeList[t]->at(j) = true;
    //     }
    //     return probe_count;
    // }

    // int exupdate_sample_matrix_constant(int current_edge, int pnum){
    //     int probe_count = 0;
    //     for(int i=current_edge;i<current_edge+pnum;i++){
    //         int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
    //         if(is_prime[i]){
    //             ProbedPrimeG[s]->push_back(t);
    //             ProbedPrimeG[t]->push_back(s);
    //         }
    //         unsigned int j=0;
    //         for(;j < g->EdgeList[s]->size();j++)
    //             if(g->EdgeList[s]->at(j) == t) break;
    //         if(!g->ProbeEdgeList[s]->at(j)){
    //             probe_count++;
    //             g->ProbeEdgeList[s]->at(j) = true;
    //         }
    //         j=0;
    //         for(;j < g->EdgeList[t]->size();j++)
    //             if(g->EdgeList[t]->at(j) == s) break;
    //         g->ProbeEdgeList[t]->at(j) = true;
    //     }
    //     return probe_count;
    // }

    // void exupdate_sample_matrix(int current_edge, int pnum){
    //     for(int i=current_edge;i<current_edge+pnum;i++){
    //         int s = order0->at((unsigned int)i), t = order1->at((unsigned int)i);
    //         if(is_prime[i]){
    //             ProbedPrimeG[s]->push_back(t);
    //             ProbedPrimeG[t]->push_back(s);
    //         }
    //         unsigned int j=0;
    //         for(;j < g->EdgeList[s]->size();j++)
    //             if(g->EdgeList[s]->at(j) == t) break;
    //         g->ProbeEdgeList[s]->at(j) = true;
    //         j=0;
    //         for(;j < g->EdgeList[t]->size();j++)
    //             if(g->EdgeList[t]->at(j) == s) break;
    //         g->ProbeEdgeList[t]->at(j) = true;
    //     }
    // }

    void Rsubgraph_clear(){ for (auto &i : Rsubgraph) i->clear();}

    void sample_matrix_clear(){ for(auto &i : this->sampled_matrix) for(unsigned int j=0;j<i->size();j++) i->at(j) = false;}

    std::pair<double, int> get_sample_DSD_induced(std::vector<int>* sampled_dsd_nodes){
        int induced_probe_count = 0;

        if(sampled_dsd_nodes == nullptr) std::cout<<"sampled_dsd_nodes is nullptr"<<std::endl;
        // std::cout<<"sampled_dsd_nodes->size():"<<sampled_dsd_nodes->size()<<std::endl;
        if(sampled_dsd_nodes->empty()) return {0.0, 0.0};

        auto sampled_dsd_induced = new std::vector<std::vector<int>*>();
        for(unsigned int i=0;i<g->EdgeList.size();i++)
            sampled_dsd_induced->push_back(new std::vector<int>());

        int sampled_dsd_induced_edge_num = 0;

        for(unsigned int i: *sampled_dsd_nodes){
            for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
                int nbr = g->EdgeList[i]->at(j);
                if(std::find(sampled_dsd_nodes->begin(), sampled_dsd_nodes->end(), nbr) != sampled_dsd_nodes->end()){
                    if(!g->ProbeEdgeList[i]->at(j)){
                        induced_probe_count++;
                        g->ProbeEdgeList[i]->at(j) = true;
                        unsigned int l = 0;
                        for(;l < g->EdgeList[nbr]->size();l++)
                            if(g->EdgeList[nbr]->at(l) == i) break;
                        g->ProbeEdgeList[nbr]->at(l) = true;
                    }
                    if(g->PrimeEdgeList[i]->at(j)){
                        sampled_dsd_induced->at(i)->push_back(nbr);
                        sampled_dsd_induced_edge_num++;
                    }
                }
            }
        }

        sampled_dsd_induced_edge_num /= 2;
        double sampled_dsd_induced_density = (sampled_dsd_induced_edge_num * 1.0) / sampled_dsd_nodes->size();

        /* only when we do not need the induced sampled_dsd*/
        for(unsigned int i=0;i<sampled_dsd_induced->size();i++)
            delete sampled_dsd_induced->at(i);
        delete sampled_dsd_induced;
        /* only when we do not need the induced sampled_dsd*/
        
        return {sampled_dsd_induced_density, induced_probe_count};
    }

    std::pair<double, int> get_Rsubgraph_induced(){
        int induce_probe_count = 0;

        auto Rsubgraph_induced = new std::vector<std::vector<int>*>();
        for(unsigned int i=0;i<g->EdgeList.size();i++)
            Rsubgraph_induced->push_back(new std::vector<int>());

        std::vector<unsigned int> Rsubgraph_nodes;
        for(unsigned int i=0;i<Rsubgraph.size();i++) if(!Rsubgraph[i]->empty()) Rsubgraph_nodes.push_back(i);

        if(Rsubgraph_nodes.empty()) return {0.0, 0.0};

        int Rsubgraph_induced_edge_num = 0;

        for(unsigned int i: Rsubgraph_nodes){
            for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
                int nbr = g->EdgeList[i]->at(j);
                if(std::find(Rsubgraph_nodes.begin(), Rsubgraph_nodes.end(), nbr)!=Rsubgraph_nodes.end()) {
                    if (!g->ProbeEdgeList[i]->at(j)) {
                        induce_probe_count++;
                        g->ProbeEdgeList[i]->at(j) = true;
                        unsigned int l = 0;
                        for (; l < g->EdgeList[nbr]->size(); l++)
                            if (g->EdgeList[nbr]->at(l) == i) break;
                        g->ProbeEdgeList[nbr]->at(l) = true;
                    }
                    if (g->PrimeEdgeList[i]->at(j)) {
                        Rsubgraph_induced->at(i)->push_back(nbr);
                        Rsubgraph_induced_edge_num++;
                    }
                }
            }
        }

        auto Rsubgraph_dsd = exact_dsd(Rsubgraph_induced);
        delete Rsubgraph_dsd.second;

        /* only when we do not need the induced Rsubgraph*/
        for(unsigned int i=0;i<Rsubgraph_induced->size();i++)
            delete Rsubgraph_induced->at(i);
        delete Rsubgraph_induced;
        /* only when we do not need the induced Rsubgraph*/
        
        return {Rsubgraph_dsd.first, induce_probe_count};
    }
};

// class sample{
// public:
//     Graph* g;
//     unsigned int probe_count = 0;
//     unsigned int induce_probe_count = 0;

//     double sample_ratio = 0;
//     double guess_gap = 0;
//     int sample_num = 0;
//     int last_k = -1;
//     int last_sample_num = 0;

//     std::vector<std::vector<bool>*> sampled_matrix;

//     std::vector<std::vector<int>*>* Rsubgraph;

//     explicit sample(Graph* _g, double _sample_ratio, double _guess_gap){
//         this->g = _g;
//         this->sample_ratio = _sample_ratio;
//         this->guess_gap = _guess_gap;
//         this->Rsubgraph = new std::vector<std::vector<int>*>();
//         for(unsigned int i=0;i<g->EdgeList.size();i++)
//             Rsubgraph->push_back(new std::vector<int>());
//         for(unsigned int i=0;i<g->EdgeList.size();i++)
//             sampled_matrix.push_back(new std::vector<bool>(g->EdgeList[i]->size(), false));
//         this->last_k = -1;
//         this->last_sample_num = 0;
//     }

//     ~sample(){
//         for (auto &i : *Rsubgraph) delete i;
//         delete Rsubgraph;
//         for(auto &i: sampled_matrix) delete i;
//     }

//     void stacked_edge_sampling_df(double density_guess){
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<> interval(0.0, 1.0);

//         std::vector<double> rnum, srnum;
//         double lim;

//         int count = 0;

//         int k = 0;
//         while(density_guess < g->DFDensities[k] && k < g->DFDensities.size() - 1) k++;

//         //std::cout<<"k:"<<k<<std::endl;

//         int last_sample_num_increase = 0;

//         int shell_sample_num = 0;
//         int shell_size = 0;
//         if(last_k == -1) shell_size = g->DFEdgeNum[k];
//         else shell_size = g->DFEdgeNum[k] - g->DFEdgeNum[last_k];

//         //std::cout<<"shell_size:"<<shell_size<<std::endl;

//         shell_sample_num = int(shell_size * sample_ratio / density_guess);

//         //std::cout<<"shell_sample_num:"<<shell_sample_num<<std::endl;

//         if(shell_size > 0){
//             rnum.resize((unsigned int)shell_size);
//             srnum.resize((unsigned int)shell_size);
//             for(unsigned int i=0;i<rnum.size();i++){
//                 rnum[i] = interval(gen);
//                 srnum[i] = rnum[i];
//             }
//             std::sort(srnum.begin(), srnum.end());
//             lim = srnum[shell_sample_num];

//             count = 0;
//             for(unsigned int node = 0;node < g->EdgeList.size();node++)
//                 for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
//                     int nbr = g->EdgeList[node]->at(j);
//                     if(nbr > node){
//                         if(g->DFDNumber[nbr] <= k && g->DFDNumber[node] <= k &&
//                            (last_k == -1 || g->DFDNumber[nbr] > last_k || g->DFDNumber[node] > last_k)){
//                             if(rnum[count] < lim){
//                                 if(!g->ProbeEdgeList[node]->at(j)){
//                                     probe_count++;
//                                     g->ProbeEdgeList[node]->at(j) = true;
//                                     unsigned int l=0;
//                                     for(;l<g->EdgeList[nbr]->size();l++){
//                                         if(g->EdgeList[nbr]->at(l) == node) break;
//                                     }
//                                     g->ProbeEdgeList[nbr]->at(l) = true;
//                                 }
//                                 if(g->PrimeEdgeList[node]->at(j)){
//                                     Rsubgraph->at(node)->push_back(nbr);
//                                     Rsubgraph->at((unsigned int)nbr)->push_back(node);
//                                 }
//                                 last_sample_num_increase++;
//                                 sampled_matrix[node]->at(j) = true;

//                                 //std::cout<<"sampled_edge: ("<<node<<", "<<nbr<<")"<<std::endl;////////////////////

//                                 unsigned int l=0;
//                                 for(;l < g->EdgeList[nbr]->size();l++){
//                                     if(g->EdgeList[nbr]->at(l) == node) break;
//                                 }
//                                 sampled_matrix[nbr]->at(l) = true;
//                             }
//                             count++;
//                         }
//                     }
//                 }
//         }
//         // make up core for the last guess
//         if(last_k == -1){
//             last_k = k;
//             last_sample_num += last_sample_num_increase;
//             //std::cout<<"$$$$$$$$$$$$$ no last k"<<std::endl;
//             return;
//         }

//         //std::cout<<"$$$$$$$$$$$$$$$$$ stack_sampling"<<std::endl;

//         //std::cout<<"last_sample_num:"<<last_sample_num<<std::endl;

//         int stack_sample_num = int(g->DFEdgeNum[last_k] * sample_ratio / density_guess) - last_sample_num;

//         //std::cout<<"stack_sample_num:"<<stack_sample_num<<std::endl;

//         int stack_core_size = g->DFEdgeNum[last_k] - last_sample_num;

//         //std::cout<<"stack_core_size:"<<stack_core_size<<std::endl;

//         rnum.clear();
//         srnum.clear();
//         rnum.resize((unsigned int)stack_core_size);
//         srnum.resize((unsigned int)stack_core_size);

//         for(unsigned int i=0;i<rnum.size();i++){
//             rnum[i] = interval(gen);
//             srnum[i] = rnum[i];
//         }
//         std::sort(srnum.begin(), srnum.end());
//         lim = srnum[stack_sample_num];

//         count = 0;
//         for(unsigned int node = 0;node < g->EdgeList.size();node++)
//             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
//                 if(sampled_matrix[node]->at(j)) continue;
//                 int nbr = g->EdgeList[node]->at(j);
//                 if(nbr > node && g->DFDNumber[nbr] <= last_k && g->DFDNumber[node] <= last_k){
//                     if(rnum[count] < lim){
//                         if(!g->ProbeEdgeList[node]->at(j)){
//                             probe_count++;
//                             g->ProbeEdgeList[node]->at(j) = true;
//                             unsigned int l=0;
//                             for(;l<g->EdgeList[nbr]->size();l++)
//                                 if(g->EdgeList[nbr]->at(l) == node) break;
//                             g->ProbeEdgeList[nbr]->at(l) = true;
//                         }
//                         if(g->PrimeEdgeList[node]->at(j)){
//                             Rsubgraph->at(node)->push_back(nbr);
//                             Rsubgraph->at((unsigned int)nbr)->push_back(node);
//                         }
//                         last_sample_num_increase++;
//                         sampled_matrix[node]->at(j) = true;

//                         //std::cout<<"sampled_edge: ("<<node<<", "<<nbr<<")"<<std::endl;////////////////////

//                         unsigned int l=0;
//                         for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
//                         sampled_matrix[nbr]->at(l) = true;
//                     }
//                     count++;
//                 }
//             }
//         last_k = k;
//         last_sample_num += last_sample_num_increase;
//     }

//     void stacked_edge_sampling(double density_guess){
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<> interval(0.0, 1.0);

//         std::vector<double> rnum, srnum;
//         double lim;

//         int count = 0;

//         int k = (int)ceil(density_guess-0.000000001);

//         int last_sample_num_increase = 0;

//         // sample from the shell
//         int shell_sample_num = 0;
//         int shell_size = 0;
//         if(last_k == -1) shell_size = g->CoreEdgeNum[k];
//         else shell_size = g->CoreEdgeNum[k] - g->CoreEdgeNum[last_k];

//         //std::cout<<"k:"<<k<<std::endl;

//         //std::cout<<"shell_size:"<<shell_size<<std::endl;

//         shell_sample_num = int(shell_size * sample_ratio / density_guess);

//         //std::cout<<"sample_ratio:"<<sample_ratio<<std::endl;

//         //std::cout<<"shell_sample_num:"<<shell_sample_num<<std::endl;

//         if(shell_size > 0){
//             rnum.resize((unsigned int)shell_size);
//             srnum.resize((unsigned int)shell_size);
//             for(unsigned int i=0;i<rnum.size();i++){
//                 rnum[i] = interval(gen);
//                 srnum[i] = rnum[i];
//             }
//             std::sort(srnum.begin(), srnum.end());
//             lim = srnum[shell_sample_num];

//             count = 0;
//             for(unsigned int node = 0;node < g->EdgeList.size();node++)
//                 for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
//                     int nbr = g->EdgeList[node]->at(j);
//                     if(nbr > node){
//                         if(g->CoreNumber[nbr] >= k && g->CoreNumber[node] >= k &&
//                            (last_k == -1 || g->CoreNumber[nbr] < last_k || g->CoreNumber[node] < last_k)){
//                             if(rnum[count] < lim){
//                                 if(!g->ProbeEdgeList[node]->at(j)){
//                                     probe_count++;
//                                     g->ProbeEdgeList[node]->at(j) = true;
//                                     unsigned int l=0;
//                                     for(;l<g->EdgeList[nbr]->size();l++)
//                                         if(g->EdgeList[nbr]->at(l) == node) break;
//                                     g->ProbeEdgeList[nbr]->at(l) = true;
//                                 }
//                                 if(g->PrimeEdgeList[node]->at(j)){
//                                     Rsubgraph->at(node)->push_back(nbr);
//                                     Rsubgraph->at((unsigned int)nbr)->push_back(node);
//                                 }
//                                 last_sample_num_increase++;
//                                 sampled_matrix[node]->at(j) = true;

//                                 //std::cout<<"sampled_edge: ("<<node<<", "<<nbr<<")"<<std::endl;////////////////////

//                                 unsigned int l=0;
//                                 for(;l < g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
//                                 sampled_matrix[nbr]->at(l) = true;
//                             }
//                             count++;
//                         }
//                     }
//                 }
//         }
//         // make up core for the last guess
//         if(last_k == -1){
//             last_k = k;
//             last_sample_num += last_sample_num_increase;
//             //std::cout<<"$$$$$$$$$$$$$ no last k"<<std::endl;
//             return;
//         }

//         //std::cout<<"$$$$$$$$$$$$$$$$$ stack_sampling"<<std::endl;

//         //std::cout<<"last_sample_num:"<<last_sample_num<<std::endl;

//         int stack_sample_num = int(g->CoreEdgeNum[last_k] * sample_ratio / density_guess) - last_sample_num;

//         //std::cout<<"stack_sample_num:"<<stack_sample_num<<std::endl;

//         int stack_core_size = g->CoreEdgeNum[last_k] - last_sample_num;

//         //std::cout<<"stack_core_size:"<<stack_core_size<<std::endl;

//         rnum.clear();
//         srnum.clear();
//         rnum.resize((unsigned int)stack_core_size);
//         srnum.resize((unsigned int)stack_core_size);

//         for(unsigned int i=0;i<rnum.size();i++){
//             rnum[i] = interval(gen);
//             srnum[i] = rnum[i];
//         }
//         std::sort(srnum.begin(), srnum.end());
//         lim = srnum[stack_sample_num];

//         //std::cout<<"rnum:";
//         //for(unsigned int i=0;i<rnum.size();i++) std::cout<<rnum[i]<<" "; std::cout<<std::endl;

//         //std::cout<<"srnum:";
//         //for(unsigned int i=0;i<srnum.size();i++) std::cout<<srnum[i]<<" "; std::cout<<std::endl;

//         //std::cout<<"lim:"<<lim<<std::endl;

//         //std::cout<<"last_k:"<<last_k<<std::endl;

//         count = 0;
//         for(unsigned int node = 0;node < g->EdgeList.size();node++)
//             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
//                 if(sampled_matrix[node]->at(j)) continue;
//                 int nbr = g->EdgeList[node]->at(j);
//                 if(nbr > node && g->CoreNumber[nbr] >= last_k && g->CoreNumber[node] >= last_k){
//                     if(rnum[count] < lim){
//                         if(!g->ProbeEdgeList[node]->at(j)){
//                             probe_count++;
//                             g->ProbeEdgeList[node]->at(j) = true;
//                             unsigned int l=0;
//                             for(;l<g->EdgeList[nbr]->size();l++)
//                                 if(g->EdgeList[nbr]->at(l) == node) break;
//                             g->ProbeEdgeList[nbr]->at(l) = true;
//                         }
//                         if(g->PrimeEdgeList[node]->at(j)){
//                             Rsubgraph->at(node)->push_back(nbr);
//                             Rsubgraph->at((unsigned int)nbr)->push_back(node);
//                         }
//                         last_sample_num_increase++;
//                         sampled_matrix[node]->at(j) = true;

//                         //std::cout<<"sampled_edge: ("<<node<<", "<<nbr<<")"<<std::endl;////////////////////

//                         unsigned int l=0;
//                         for(;l<g->EdgeList[nbr]->size();l++) if(g->EdgeList[nbr]->at(l) == node) break;
//                         sampled_matrix[nbr]->at(l) = true;
//                     }
//                     count++;
//                 }
//             }
//         last_k = k;
//         last_sample_num += last_sample_num_increase;
//     }

//     void edge_sampling(){
//         /* get Rsubgraph by sampling |num| edges */
//         std::vector<double> rnum((unsigned int)g->ValidEdgeNum, 0.0);
//         std::vector<double> srnum((unsigned int)g->ValidEdgeNum, 0.0); // sorted rnum

//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<> interval(0.0, 1.0);

//         for(int i = 0; i< g->ValidEdgeNum;i++) {
//             rnum[i] = interval(gen);
//             srnum[i] = rnum[i];
//         }

//         //std::cout<<"rnum:";
//         //for(unsigned int i=0;i<rnum.size();i++) std::cout<<rnum[i]<<" "; std::cout<<std::endl;

//         std::sort(srnum.begin(), srnum.end());

//         double lim = srnum[sample_num]; // sample edge i with (rnum[i] < lim)

//         //std::cout<<"snum:";
//         //for(unsigned int i=0;i<srnum.size();i++) std::cout<<srnum[i]<<" "; std::cout<<std::endl;

//         //std::cout<<"lim:"<<lim<<std::endl;

//         int count = 0;
//         for(unsigned int node=0;node<g->EdgeList.size();node++)
//             for(unsigned int j=0;j<g->EdgeList[node]->size();j++){
//                 int nbr = g->EdgeList[node]->at(j);
//                 if(nbr > node && g->ValidEdgeList[node]->at(j)){
//                     if(rnum[count] < lim){
//                         if(!g->ProbeEdgeList[node]->at(j)){
//                             probe_count++;

//                             g->ProbeEdgeList[node]->at(j) = true;
//                             unsigned int l=0;
//                             for(;l<g->EdgeList[nbr]->size();l++)
//                                 if(g->EdgeList[nbr]->at(l) == node) break;
//                             g->ProbeEdgeList[nbr]->at(l) = true;
//                         }
//                         if(g->PrimeEdgeList[node]->at(j)) {
//                             Rsubgraph->at(node)->push_back(nbr);
//                             Rsubgraph->at((unsigned int)nbr)->push_back(node);
//                         }
//                     }
//                     count++;
//                 }
//             }
//     }

//     std::pair<double, std::vector<int>*> execute2(){
//         std::cout<<std::endl<<std::endl<<std::endl;

//         std::cout<<"1st ROUND:"<<std::endl;

//         double density_guess = g->CoreEdgeNum.size() - 1;
//         std::cout<<"density_guess:"<<density_guess<<std::endl;

//         this->sample_num = int(g->CoreEdgeNum[int(ceil(density_guess-0.000000001))] * sample_ratio / density_guess);

//         std::cout<<"sample_num:"<<this->sample_num<<std::endl;

//         int kvalue = (int)ceil(density_guess - 0.000000001);
//         std::cout<<"kvalue:"<<kvalue<<std::endl;

//         g->set_valid_edge(kvalue);

//         edge_sampling();
//         std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//         auto Rsubgraph_induced_res = get_Rsubgraph_induced();
//         auto exact_res = exact_dsd(Rsubgraph_induced_res.second);
//         double Rsubgraph_density = exact_res.first;

//         std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//         std::cout<<std::endl;

//         while(Rsubgraph_density < density_guess){
//             delete exact_res.second;
//             for (auto &i : *Rsubgraph_induced_res.second) delete i;
//             delete Rsubgraph_induced_res.second;
//             Rsubgraph_clear();

//             std::cout<<"next ROUND:"<<std::endl;

//             density_guess /= guess_gap;

//             std::cout<<"density_guess:"<<density_guess<<std::endl;

//             this->sample_num = int(g->CoreEdgeNum[int(ceil(density_guess-0.000000001))] * sample_ratio / density_guess);
//             std::cout<<"sample_num:"<<this->sample_num<<std::endl;

//             kvalue = (int)ceil(density_guess-0.000000001);
//             std::cout<<"kvalue:"<<kvalue<<std::endl;

//             g->set_valid_edge(kvalue);
//             edge_sampling();
//             std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//             Rsubgraph_induced_res = get_Rsubgraph_induced();
//             exact_res = exact_dsd(Rsubgraph_induced_res.second);
//             Rsubgraph_density = exact_res.first;

//             std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//             std::cout<<std::endl;
//         }
//         for (auto &i : *Rsubgraph_induced_res.second) delete i;
//         delete Rsubgraph_induced_res.second;
//         std::cout<<"induced_probe_count:"<<induce_probe_count<<std::endl;
//         return exact_res;
//     }

//     std::pair<double, std::vector<int>*> stacked_execute_df(){
//         //std::cout<<std::endl<<std::endl<<"1st ROUND:"<<std::endl;

//         double density_guess = g->df_max - 0.000000001;
//         //std::cout<<"density_guess:"<<density_guess<<std::endl;

//         stacked_edge_sampling_df(density_guess);
//         //std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//         //std::cout<<"Rsubgraph:"<<std::endl;
//         //for(unsigned int n=0;n<Rsubgraph->size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//         //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}
//         //std::cout<<std::endl;

//         //std::cout<<"sampled_matrix:"<<std::endl;
//         //for(unsigned int n=0;n<sampled_matrix.size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
//         //        std::cout<<sampled_matrix[n]->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}

//         auto Rsubgraph_induced_res = get_Rsubgraph_induced();
//         double Rsubgraph_density = Rsubgraph_induced_res.first;

//         //std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//         //std::cout<<std::endl;

//         while(Rsubgraph_density < density_guess){
//             for(auto &i : *Rsubgraph_induced_res.second) delete i;
//             delete Rsubgraph_induced_res.second;

//             //std::cout<<"next ROUND:"<<std::endl;

//             density_guess /= guess_gap;
//             //std::cout<<"density_guess:"<<density_guess<<std::endl;

//             stacked_edge_sampling_df(density_guess);

//             //std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//             //std::cout<<"Rsubgraph:"<<std::endl;
//             //for(unsigned int n=0;n<Rsubgraph->size();n++){
//             //    std::cout<<"n"<<n<<": ";
//             //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//             //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//             //    std::cout<<std::endl;
//             //}
//             //std::cout<<std::endl;

//             //std::cout<<"sampled_matrix:"<<std::endl;
//             //for(unsigned int n=0;n<sampled_matrix.size();n++){
//             //    std::cout<<"n"<<n<<": ";
//             //    for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
//             //        std::cout<<sampled_matrix[n]->at(i)<<" ";
//             //    std::cout<<std::endl;
//             //}

//             Rsubgraph_induced_res = get_Rsubgraph_induced();
//             Rsubgraph_density = Rsubgraph_induced_res.first;

//             //std::cout<<"Rsubgraph_density:"<<Rsubgraph_density << std::endl;
//             std::cout<<std::endl;
//         }
//         auto exact_res = exact_dsd(Rsubgraph_induced_res.second);
//         for (auto &i : *Rsubgraph_induced_res.second) delete i;
//         delete Rsubgraph_induced_res.second;
//         //std::cout<<"induced_probe_count:"<<induce_probe_count<<std::endl;
//         return exact_res;
//     }

//     std::pair<double, std::vector<int>*> stacked_execute(){
//         std::cout<<std::endl<<std::endl<<"1st ROUND:"<<std::endl;

//         double density_guess = g->CoreEdgeNum.size() - 1;
//         std::cout<<"density_guess:"<<density_guess<<std::endl;

//         stacked_edge_sampling(density_guess);
//         std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//         //std::cout<<"Rsubgraph:"<<std::endl;
//         //for(unsigned int n=0;n<Rsubgraph->size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//         //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}
//         //std::cout<<std::endl;

//         //std::cout<<"sampled_matrix:"<<std::endl;
//         //for(unsigned int n=0;n<sampled_matrix.size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
//         //        std::cout<<sampled_matrix[n]->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}

//         auto Rsubgraph_induced_res = get_Rsubgraph_induced();
//         double Rsubgraph_density = Rsubgraph_induced_res.first;

//         //std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//         //std::cout<<std::endl;

//         while(Rsubgraph_density < density_guess){
//             for(auto &i : *Rsubgraph_induced_res.second) delete i;
//             delete Rsubgraph_induced_res.second;

//             std::cout<<"next ROUND:"<<std::endl;

//             density_guess /= guess_gap;
//             std::cout<<"density_guess:"<<density_guess<<std::endl;

//             stacked_edge_sampling(density_guess);

//             std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//             //std::cout<<"Rsubgraph:"<<std::endl;
//             //for(unsigned int n=0;n<Rsubgraph->size();n++){
//             //    std::cout<<"n"<<n<<": ";
//             //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//             //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//             //    std::cout<<std::endl;
//             //}
//             //std::cout<<std::endl;

//             //std::cout<<"sampled_matrix:"<<std::endl;
//             //for(unsigned int n=0;n<sampled_matrix.size();n++){
//             //    std::cout<<"n"<<n<<": ";
//             //    for(unsigned int i=0;i<sampled_matrix[n]->size();i++)
//             //        std::cout<<sampled_matrix[n]->at(i)<<" ";
//             //    std::cout<<std::endl;
//             //}

//             Rsubgraph_induced_res = get_Rsubgraph_induced();
//             Rsubgraph_density = Rsubgraph_induced_res.first;

//             std::cout<<"Rsubgraph_density:"<<Rsubgraph_density << std::endl;
//             std::cout<<std::endl;
//         }
//         auto exact_res = exact_dsd(Rsubgraph_induced_res.second);
//         for (auto &i : *Rsubgraph_induced_res.second) delete i;
//         delete Rsubgraph_induced_res.second;
//         std::cout<<"induced_probe_count:"<<induce_probe_count<<std::endl;
//         return exact_res;
//     }

//     std::pair<double, std::vector<int>*> execute(){
//         std::cout<<std::endl<<std::endl<<"1st ROUND:"<<std::endl;

//         double density_guess = g->CoreEdgeNum.size() - 1;
//         std::cout<<"density_guess:"<<density_guess<<std::endl;

//         this->sample_num = int(g->CoreEdgeNum[int(ceil(density_guess-0.000000001))] * sample_ratio / density_guess);

//         std::cout<<"sample_num:"<<this->sample_num<<std::endl;

//         int kvalue = (int)ceil(density_guess-0.000000001);
//         std::cout<<"kvalue:"<<kvalue<<std::endl;

//         g->set_valid_edge(kvalue);

//         edge_sampling();
//         std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//         //std::cout<<"Rsubgraph:"<<std::endl;
//         //for(unsigned int n=0;n<Rsubgraph->size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//         //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}
//         //std::cout<<std::endl;

//         auto Rsubgraph_induced_res = get_Rsubgraph_induced();
//         double Rsubgraph_density = Rsubgraph_induced_res.first;

//         std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//         std::cout<<std::endl;

//         while(Rsubgraph_density < density_guess){
//             for (auto &i : *Rsubgraph_induced_res.second) delete i;
//             delete Rsubgraph_induced_res.second;
//             Rsubgraph_clear();

//             std::cout<<"next ROUND:"<<std::endl;

//             density_guess /= guess_gap;
//             std::cout<<"density_guess:"<<density_guess<<std::endl;

//             this->sample_num = int(g->CoreEdgeNum[int(ceil(density_guess-0.000000001))] * sample_ratio / density_guess);
//             std::cout<<"sample_num:"<<this->sample_num<<std::endl;

//             kvalue = (int)ceil(density_guess-0.000000001);
//             std::cout<<"kvalue:"<<kvalue<<std::endl;

//             g->set_valid_edge(kvalue);
//             edge_sampling();
//             std::cout<<"tmp_probe_count:"<<probe_count<<std::endl;

//             //std::cout<<"Rsubgraph:"<<std::endl;
//             //for(unsigned int n=0;n<Rsubgraph->size();n++){
//             //    std::cout<<"n"<<n<<": ";
//             //for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//             //    std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//             //std::cout<<std::endl;
//             //}
//             //std::cout<<std::endl<<std::endl;

//             Rsubgraph_induced_res = get_Rsubgraph_induced();
//             Rsubgraph_density = Rsubgraph_induced_res.first;

//             std::cout<<"Rsubgraph_density:"<<Rsubgraph_density<<std::endl;
//             std::cout<<std::endl;
//         }
//         auto exact_res = exact_dsd(Rsubgraph_induced_res.second);
//         for (auto &i : *Rsubgraph_induced_res.second) delete i;
//         delete Rsubgraph_induced_res.second;
//         std::cout<<"induced_probe_count:"<<induce_probe_count<<std::endl;
//         return exact_res;
//     }

//     std::pair<double, std::vector<int>*> run(){
//         /*return the density of subgraph discovered by edge-sampling*/

//         this->sample_num = int(g->EdgeNum * sample_ratio);

//         edge_sampling();

//         //std::cout<<"Rsubgraph:"<<std::endl;
//         //for(unsigned int n=0;n<Rsubgraph->size();n++){
//         //    std::cout<<"n"<<n<<": ";
//         //    for(unsigned int i=0;i<Rsubgraph->at(n)->size();i++)
//         //        std::cout<<Rsubgraph->at(n)->at(i)<<" ";
//         //    std::cout<<std::endl;
//         //}
//         //std::cout<<std::endl<<std::endl;

//         auto Rsubgraph_ds = exact_dsd(Rsubgraph);

//         double edge_sampling_density = induced_density(g, Rsubgraph_ds.second);
//         return {edge_sampling_density, Rsubgraph_ds.second};
//     }

//     std::pair<double, std::vector<std::vector<int>*>*> get_Rsubgraph_induced(){
//         auto Rsubgraph_induced = new std::vector<std::vector<int>*>();
//         for(unsigned int i=0;i<g->EdgeList.size();i++)
//             Rsubgraph_induced->push_back(new std::vector<int>());

//         std::vector<unsigned int> Rsubgraph_nodes;
//         for(unsigned int i=0;i<Rsubgraph->size();i++)
//             if(!Rsubgraph->at(i)->empty()) Rsubgraph_nodes.push_back(i);

//         if(Rsubgraph_nodes.empty()) return {0.0, Rsubgraph_induced};

//         int Rsubgraph_induced_edge_num = 0;

//         for(unsigned int i: Rsubgraph_nodes){
//             for(unsigned int j=0;j<g->EdgeList[i]->size();j++){
//                 int nbr = g->EdgeList[i]->at(j);
//                 if(std::find(Rsubgraph_nodes.begin(), Rsubgraph_nodes.end(), nbr)!=Rsubgraph_nodes.end()) {
//                     if (!g->ProbeEdgeList[i]->at(j)) {
//                         probe_count++;
//                         induce_probe_count++;
//                         g->ProbeEdgeList[i]->at(j) = true;
//                         unsigned int l = 0;
//                         for (; l < g->EdgeList[nbr]->size(); l++)
//                             if (g->EdgeList[nbr]->at(l) == i) break;
//                         g->ProbeEdgeList[nbr]->at(l) = true;
//                     }
//                     if (g->PrimeEdgeList[i]->at(j)) {
//                         Rsubgraph_induced->at(i)->push_back(nbr);
//                         Rsubgraph_induced_edge_num++;
//                     }
//                 }
//             }
//         }

//         Rsubgraph_induced_edge_num /= 2;
//         double Rsubgraph_induced_density = (Rsubgraph_induced_edge_num * 1.0) / Rsubgraph_nodes.size();
//         return {Rsubgraph_induced_density, Rsubgraph_induced};
//     }

//     void Rsubgraph_clear(){ for (auto &i : *Rsubgraph) i->clear();}
// };