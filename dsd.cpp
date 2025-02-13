#ifndef DSD_H
#define DSD_H

#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unordered_map>
#include "flowgraph.cpp"
#include "maxflow.cpp"

std::vector<unsigned int>* FlowComputation(std::vector<std::vector<int>*>* g,
                                           double rho, int edge_num){
    auto source_seg = new std::vector<unsigned int>();
    auto fg = new FlowGraph<double, double, double>(g->size(), edge_num);
    for(unsigned int i=0;i<g->size();i++) fg->add_node();
    for(unsigned int i=0;i<g->size();i++)
        fg->add_tweights(i, edge_num, edge_num + 2*rho - g->at(i)->size());
    for(unsigned int i=0;i<g->size();i++){
        for(unsigned int j=0;j<g->at(i)->size();j++){
            auto n = (unsigned int)g->at(i)->at(j);
            if(n > i){
                fg->add_edge(i, n, 1, 1);
            }
        }
    }
    double flow = fg->maxflow();
    for(unsigned int i=0;i<g->size();i++){
        if(fg->what_segment(i) == FlowGraph<double, double, double>::SOURCE)
            source_seg->push_back(i);
    }
    delete fg;
    return source_seg;
}

std::pair<double, double> core_based_dsd(std::vector<std::vector<int>*>* g, const std::string &rname){
    std::ofstream ofile;
    ofile.open("temp_graph"+rname+".txt");

    unsigned int ecount = 0;
    for(unsigned int i=0;i<g->size();i++)
        ecount += g->at(i)->size();
    ecount /= 2;
    ofile << g->size() << " " << ecount<<std::endl;

    for(unsigned int i=0;i<g->size();i++)
        for(unsigned int j=0;j<g->at(i)->size();j++)
            ofile << i <<" "<<g->at(i)->at(j)<<std::endl;
    ofile.close();

    std::string cmd1 = "./DensestSubgraph -path temp_graph"+rname+".txt -t u -a e -red k-core -alloc flow-exact -ext flow-exact -ver flow-exact > temp_dsd"+rname+".txt";

    system(cmd1.c_str());

    std::ifstream rfile;
    rfile.open("temp_dsd"+rname+".txt");
    std::string line;
    getline(rfile, line);
    double ds = std::stod(line);
    getline(rfile, line);
    double ds_time = std::stod(line);
    rfile.close();

    std::string cmd2 = "rm temp_graph"+rname+".txt";
    std::string cmd3 = "rm temp_dsd"+rname+".txt";

    system(cmd2.c_str());
    system(cmd3.c_str());
    return {ds, ds_time};
}

std::pair<double, std::vector<int>*> exact_dsd(std::vector<std::vector<int>*>* g){
    unsigned int edge_num = 0;
    for (auto &i : *g) edge_num += i->size();

    auto subgraph = new std::vector<int>();

    double min_deg = 0;
    double max_deg = 0;
    for (auto &i : *g) {
        if(i->size() > max_deg)
            max_deg = i->size() - 1;
    }

    while(max_deg - min_deg >= 0.0000000001){
        double least_density = (max_deg +  min_deg) / 2;
        std::vector<unsigned int>* source_segment = FlowComputation(g, least_density, edge_num);
        if(source_segment->empty()){
            max_deg = least_density;
        }
        else{
            subgraph->clear();
            min_deg = least_density;
            for(unsigned int i: *source_segment) subgraph->push_back((int)i);
        }
        delete source_segment;
    }
    return {max_deg, subgraph};
}

#endif