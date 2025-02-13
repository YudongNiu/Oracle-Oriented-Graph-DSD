#include <iostream>
#include "graph.cpp"
#include "OrderedProbe.cpp"
#include "SAMPLE.cpp"

int main(int argc, char *argv[]) {
    auto dataset = std::string(argv[1]);
    auto ranking = std::string(argv[2]);
    /* ranking methods: wedge, triangle, core, core_static, wedge_static, triangle_static, truss_static, df, soe*/

    auto executable = std::string(argv[3]); /* including: exp; lin; bin; exp2*/

    auto approx_ratio = std::stod(argv[4]); /* valid when executable=="bin" */
    auto expand = std::stod(argv[5]); /* valid when executable=="exp"||"exp2" */
    auto linear_ratio = std::stod(argv[6]); /* valid when executable=="lin" */
    auto shuffle = std::string(argv[7]); /* whether to shuffle the order with same priority value*/

    Graph* g;
    if(dataset == "amazon" || dataset == "dblp" || dataset == "youtube" || dataset == "orkut" || dataset == "lj" || dataset =="friendster")
        g = new Graph(dataset);
    else g = new Graph(dataset, false); /* signed networks*/

//    g->printPrimeEdges();

    auto order0 = new std::vector<int>();
    auto order1 = new std::vector<int>();

    std::string order_file = "datasets/" + dataset + "."+ranking+"_order";
    std::ifstream order_in;
    order_in.open(order_file);
    std::string order_line;
    while(getline(order_in, order_line)){
        std::stringstream ss0(order_line);
        std::string temp_str;
        std::getline(ss0, temp_str, ' ');
        order0->push_back(stoi(temp_str));
        std::getline(ss0, temp_str, ' ');
        order1->push_back(stoi(temp_str));
    }

    g->core();

    if(dataset == "amazon" || dataset == "dblp" || dataset == "youtube" || dataset == "orkut" || dataset == "lj" || dataset =="friendster") {
        std::string community_file = "datasets/"+dataset + ".cmty_filter";
        std::ifstream community_in;
        community_in.open(community_file);
        std::string community_line;

        auto community = new std::unordered_map<unsigned int, bool>();

        std::vector<double> avg_probe_ratios(10, 0.0);
        std::vector<double> avg_running_times(10, 0.0);
        std::vector<double> avg_epsilons(10, 0.0);

        std::vector<double> avg_ubs;
        std::vector<double> avg_lbs;

        double avg_probe_ratio = 0.0;
        std::vector<double> avg_cumulative(1000, 0.0);

        int count = 0;
        while (getline(community_in, community_line)) {
            std::stringstream ss0(community_line);
            std::string temp_str;
            while (std::getline(ss0, temp_str, '\t'))
                community->insert({(unsigned int) stoi(temp_str), true});
            g->setPrime(community);

            if (ranking != "soe"){
                oprobe baseline(g, order0, order1, ranking);

                if(shuffle == "y") baseline.TieShuffle(ranking);

                if(executable == "exp") {
                    auto res = baseline.run_exp_coreexact_vary_eps(expand, 0.01);
                    std::vector<double> probe_ratios;
                    for (double i : *res.first) probe_ratios.push_back(i * 1.0 / g->EdgeNum);

                    std::cout<<"probe_ratios: ";
                    for (double probe_ratio : probe_ratios) std::cout<< probe_ratio <<" "; std::cout<<std::endl;
                    std::cout<<"times: ";
                    for (double i : *res.second) std::cout<< i <<" "; std::cout<<std::endl;
                    for(unsigned int i=0;i<probe_ratios.size();i++) {
                        avg_probe_ratios[i] += probe_ratios[i];
                        avg_running_times[i] += res.second->at(i);
                    }
                    delete res.first;
                    delete res.second;
                }
                else if(executable == "exp2"){
                    auto res = baseline.run_exp_coreexact_vary_pcost(expand, 0.01);

                    std::cout<<"lbs: ";
                    for(unsigned int i=0;i<baseline.lbs.size();i++) std::cout<<baseline.lbs[i]<<" "; std::cout<<std::endl;
                    std::cout<<"ubs: ";
                    for(unsigned int i=0;i<baseline.ubs.size();i++) std::cout<<baseline.ubs[i]<<" "; std::cout<<std::endl;

                    while(avg_lbs.size() < baseline.lbs.size()) avg_lbs.push_back(0);
                    while(avg_ubs.size() < baseline.ubs.size()) avg_ubs.push_back(0);

                    for(unsigned int i=0;i<baseline.ubs.size();i++) avg_ubs[i] += baseline.ubs[i];
                    for(unsigned int i=0;i<baseline.lbs.size();i++) avg_lbs[i] += baseline.lbs[i];

                    std::cout<<"epsilon: ";
                    for(double epsilon: *res) std::cout<< epsilon<<" "; std::cout<<std::endl;
                    while(res->size() > avg_probe_ratios.size()){
                        avg_probe_ratios.push_back(0);
                        avg_epsilons.push_back(0);
                    }
                    for(unsigned int i=0;i<avg_probe_ratios.size();i++){
                        if(i < res->size()) avg_epsilons[i] += res->at(i);
                        else avg_epsilons[i] += 1.0;
                    }
                    delete res;
                }
                else if(executable == "bin"){
                    int probe_count = baseline.run_binary(approx_ratio);
                    double probe_ratio = (probe_count * 1.0 / g->EdgeNum);
                    std::cout<<"probe_ratio: "<<probe_ratio<<std::endl;
                    avg_probe_ratio += probe_ratio;
                }
                else if(executable == "lin"){
                    g->getPrimeDSD();
                    auto approxes = baseline.run_linear(linear_ratio);
                    for (double approxe : *approxes) std::cout<< approxe <<" ";
                    std::cout<<std::endl;

                    while(approxes->size() < 1000) approxes->push_back(1.0);
                    for(unsigned int i=0;i<approxes->size();i++) avg_cumulative[i] += approxes->at(i);
                    delete approxes;
                }
            } else {
                soe baseline(g, "soe");

                if(shuffle == "y") baseline.TieShuffle(ranking);

                if(executable == "exp") {
                    auto res = baseline.run_exp_coreexact_vary_eps(expand, 0.01);
                    std::vector<double> probe_ratios;
                    for (double i : *res.first) probe_ratios.push_back(i * 1.0 / g->EdgeNum);
                    std::cout<<"probe_ratios: ";
                    for (double probe_ratio : probe_ratios) std::cout<< probe_ratio <<" "; std::cout<<std::endl;
                    std::cout<<"times: ";
                    for (double i : *res.second) std::cout<< i <<" "; std::cout<<std::endl;
                    for(unsigned int i=0;i<probe_ratios.size();i++) {
                        avg_probe_ratios[i] += probe_ratios[i];
                        avg_running_times[i] += res.second->at(i);
                    }
                    delete res.first;
                    delete res.second;
                }
                else if(executable == "exp2"){
                    auto res = baseline.run_exp_coreexact_vary_pcost(expand, 0.01);

                    std::cout<<"lbs: ";
                    for(unsigned int i=0;i<baseline.lbs.size();i++) std::cout<<baseline.lbs[i]<<" "; std::cout<<std::endl;
                    std::cout<<"ubs: ";
                    for(unsigned int i=0;i<baseline.ubs.size();i++) std::cout<<baseline.ubs[i]<<" "; std::cout<<std::endl;

                    while(avg_lbs.size() < baseline.lbs.size()) avg_lbs.push_back(0);
                    while(avg_ubs.size() < baseline.ubs.size()) avg_ubs.push_back(0);

                    for(unsigned int i=0;i<baseline.ubs.size();i++) avg_ubs[i] += baseline.ubs[i];
                    for(unsigned int i=0;i<baseline.lbs.size();i++) avg_lbs[i] += baseline.lbs[i];

                    std::cout<<"epsilon: ";
                    for(double epsilon: *res) std::cout<< epsilon<<" "; std::cout<<std::endl;
                    while(res->size() > avg_probe_ratios.size()){
                        avg_probe_ratios.push_back(0);
                        avg_epsilons.push_back(0);
                    }
                    for(unsigned int i=0;i<avg_probe_ratios.size();i++){
                        if(i < res->size()) avg_epsilons[i] += res->at(i);
                        else avg_epsilons[i] += 1.0;
                    }
                    delete res;
                }
                else if(executable == "bin"){
                    int probe_count = baseline.run_binary(approx_ratio);
                    double probe_ratio = (probe_count * 1.0 / g->EdgeNum);
                    std::cout<<"probe_ratio: "<<probe_ratio<<std::endl;
                    avg_probe_ratio += probe_ratio;
                }
                else if(executable == "lin"){
                    g->getPrimeDSD();
                    auto approxes = baseline.run_linear(linear_ratio);
                    for (double approxe : *approxes) std::cout<< approxe <<" ";
                    std::cout<<std::endl;

                    while(approxes->size() < 1000) approxes->push_back(1.0);
                    for(unsigned int i=0;i<approxes->size();i++) avg_cumulative[i] += approxes->at(i);
                    delete approxes;
                }
            }
            g->clear();
            community->clear();
            count++;
            if(count >= 1000) break;
        }

        avg_probe_ratio /= count;
        for (double &i : avg_cumulative) i /= count;
        for (double &avg_probe_rat : avg_probe_ratios) avg_probe_rat /= count;
        for (double &avg_running_time : avg_running_times) avg_running_time /= count;

        for(unsigned int i=0;i<avg_ubs.size();i++) avg_ubs[i] /= count;
        for(unsigned int i=0;i<avg_lbs.size();i++) avg_lbs[i] /= count;

        if(executable == "bin")
            std::cout << "~probe_ratio:" << avg_probe_ratio << std::endl;
        else if(executable == "exp")
        {
            std::cout<<"~epsilons: 1 0.98 0.96 0.94 0.92 0.9 0.8 0.7 0.6 0.5"<<std::endl;
            std::cout<<"~probe_ratios: ";
            for (double avg_pratio : avg_probe_ratios) std::cout<< avg_pratio <<" "; std::cout<<std::endl;
            std::cout<<"~times: ";
            for(double avg_time : avg_running_times) std::cout<<avg_time<<" "; std::cout<<std::endl;
        }
        else if(executable == "exp2"){
            std::cout<<"~lbs: ";
            for(unsigned int i=0;i<avg_lbs.size();i++) std::cout<<avg_lbs[i]<<" "; std::cout<<std::endl;
            std::cout<<"~ubs: ";
            for(unsigned int i=0;i<avg_ubs.size();i++) std::cout<<avg_ubs[i]<<" "; std::cout<<std::endl;

            std::cout<<"~epsilon: ";
            for (double avg_epsilon : avg_epsilons) std::cout<< (avg_epsilon/count) <<" "; std::cout<<std::endl;

            std::cout<<"~probe_ratios: ";
            int pnum = int(ceil((0.01 * g->EdgeNum)+0.000000001));
            int current_edge = 0;
            for(unsigned int i=0;i<avg_epsilons.size();i++){
                if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;
                current_edge += pnum;
                double probe_ratio = (current_edge * 1.0 / g->EdgeNum);
                std::cout<<probe_ratio<<" ";
                pnum = int(ceil(pnum * expand)+0.000000001);
            }
            std::cout<<std::endl;
        }
        else if(executable == "lin") {
            std::cout<<"~accumulative: ";
            for (double i : avg_cumulative) {
                std::cout<< i <<" "; std::cout<<std::endl;
                if (i >=0.999999999) break;
            }
        }
    }
    else{
        if(ranking != "soe"){
            oprobe baseline(g, order0, order1, ranking);

            if(shuffle == "y") baseline.TieShuffle(ranking);

            if(executable == "exp"){
                auto res = baseline.run_exp_coreexact_vary_eps(expand, 0.01);
                std::vector<double> probe_ratios;
                for (double i : *res.first) probe_ratios.push_back(i * 1.0 / g->EdgeNum);

                std::cout<<"~epsilons: 1 0.98 0.96 0.94 0.92 0.9 0.8 0.7 0.6 0.5"<<std::endl;
                std::cout<<"~probe_ratios: ";
                for (double probe_ratio : probe_ratios) std::cout<< probe_ratio <<" "; std::cout<<std::endl;
                std::cout<<"~times: ";
                for (double i : *res.second) std::cout<< i <<" "; std::cout<<std::endl;
                delete res.first;
                delete res.second;
            }
            else if(executable == "exp2"){
                auto res = baseline.run_exp_coreexact_vary_pcost(expand, 0.01);

                std::cout<<"~lbs: ";
                for(unsigned int i=0;i<baseline.lbs.size();i++) std::cout<<baseline.lbs[i]<<" "; std::cout<<std::endl;
                std::cout<<"~ubs: ";
                for(unsigned int i=0;i<baseline.ubs.size();i++) std::cout<<baseline.ubs[i]<<" "; std::cout<<std::endl;

                std::cout<<"~epsilons: ";
                for(double epsilon: *res) std::cout<< epsilon<<" "; std::cout<<std::endl;
                std::cout<<"~probe_ratios: ";
                int pnum = int(ceil((0.01 * g->EdgeNum)+0.000000001));
                int current_edge = 0;
                for(unsigned int i=0;i<res->size();i++){
                    if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;
                    current_edge += pnum;
                    double probe_ratio = (current_edge * 1.0 / g->EdgeNum);
                    std::cout<<probe_ratio<<" ";
                    pnum = int(ceil(pnum * expand)+0.000000001);
                }
                std::cout<<std::endl;
                delete res;
            }
            else if(executable == "bin"){
                int probe_count = baseline.run_binary(approx_ratio);
                double probe_ratio = (probe_count * 1.0 / g->EdgeNum);
                std::cout<<"probe_ratio: "<<probe_ratio<<std::endl;
            }
            else if(executable == "lin"){
                g->getPrimeDSD();
                auto approxes = baseline.run_linear(linear_ratio);
                for (double approxe : *approxes) std::cout<< approxe <<" ";
                std::cout<<std::endl;
            }
        }
        else{
            soe baseline(g, "soe");
            
            if(shuffle == "y") baseline.TieShuffle(ranking);

            if(executable == "exp") {
                auto res = baseline.run_exp_coreexact_vary_eps(expand, 0.01);
                std::vector<double> probe_ratios;
                for (double i : *res.first) probe_ratios.push_back(i * 1.0 / g->EdgeNum);
                std::cout<<"~epsilons: 1 0.98 0.96 0.94 0.92 0.9 0.8 0.7 0.6 0.5"<<std::endl;
                std::cout<<"~probe_ratios: ";
                for (double probe_ratio : probe_ratios) std::cout<< probe_ratio <<" "; std::cout<<std::endl;
                std::cout<<"~times: ";
                for (double i : *res.second) std::cout<< i <<" "; std::cout<<std::endl;
                delete res.first;
                delete res.second;
            }
            else if(executable == "exp2"){
                auto res = baseline.run_exp_coreexact_vary_pcost(expand, 0.01);

                std::cout<<"~lbs: ";
                for(unsigned int i=0;i<baseline.lbs.size();i++) std::cout<<baseline.lbs[i]<<" "; std::cout<<std::endl;
                std::cout<<"~ubs: ";
                for(unsigned int i=0;i<baseline.ubs.size();i++) std::cout<<baseline.ubs[i]<<" "; std::cout<<std::endl;

                std::cout<<"~epsilons: ";
                for(double epsilon: *res) std::cout<< epsilon<<" "; std::cout<<std::endl;
                std::cout<<"~probe_ratios: ";
                int pnum = int(ceil((0.01 * g->EdgeNum)+0.000000001));
                int current_edge = 0;
                for(unsigned int i=0;i<res->size();i++){
                    if(current_edge + pnum >= g->EdgeNum) pnum = g->EdgeNum-current_edge;
                    current_edge += pnum;
                    double probe_ratio = (current_edge * 1.0 / g->EdgeNum);
                    std::cout<<probe_ratio<<" ";
                    pnum = int(ceil(pnum * expand)+0.000000001);
                }
                std::cout<<std::endl;
                delete res;
            }
            else if(executable == "bin"){
                int probe_count = baseline.run_binary(approx_ratio);
                double probe_ratio = (probe_count * 1.0 / g->EdgeNum);
                std::cout<<"probe_ratio: "<<probe_ratio<<std::endl;
            }
            else if(executable == "lin"){
                g->getPrimeDSD();
                auto approxes = baseline.run_linear(linear_ratio);
                for (double approxe : *approxes) std::cout<< approxe <<" ";
                std::cout<<std::endl;
            }
        }
    }
    return 0;
}