#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include "graph.cpp"
#include <unordered_map>

double induced_density(Graph* g, std::vector<int>* subgraph){
    std::unordered_map<int, bool> subgraph_map;
    for(int i: *subgraph)
        subgraph_map.insert({i, true});

    int prime_edge_num = 0;
    for(int n: *subgraph){
        for(unsigned int i=0;i<g->EdgeList[n]->size();i++){
            int nbr = g->EdgeList[n]->at(i);
            if(subgraph_map.find(nbr)!=subgraph_map.end() && g->PrimeEdgeList[n]->at(i))
                prime_edge_num++;
        }
    }
    prime_edge_num /= 2;
    double density = (prime_edge_num * 1.0) / subgraph->size();
    return density;
}

// void groundtruth_density(std::string dataset){
//     Graph g(dataset);

//     std::string community_file = dataset + ".cmty";
//     std::ifstream community_in;
//     community_in.open(community_file);
//     std::string community_line;

//     std::string out_file = dataset + ".ground_truth_res";
//     std::ofstream resout;
//     resout.open(out_file);

//     auto community = new std::vector<int>();
//     auto community_map = new std::unordered_map<unsigned int, bool>();

//     while(getline(community_in, community_line)){
//         std::stringstream ss0(community_line);
//         std::string temp_str;
//         while(std::getline(ss0, temp_str, '\t')) {
//             community->push_back(stoi(temp_str));
//             community_map->insert({(unsigned int)stoi(temp_str), true});
//         }
//         g.setPrime(community_map);

//         auto GTsubgraph = new std::vector<std::vector<int>*>();
//         for(unsigned int i=0;i<g.EdgeList.size();i++)
//             GTsubgraph->push_back(new std::vector<int>());
//         for(int node: *community){
//             for(unsigned int i=0;i<g.EdgeList[node]->size();i++){
//                 if(g.PrimeEdgeList[node]->at(i)){
//                     int nbr = g.EdgeList[node]->at(i);
//                     GTsubgraph->at(node)->push_back(nbr);
//                     GTsubgraph->at(nbr)->push_back(node);
//                 }
//             }
//         }

//         auto res = exact_dsd(GTsubgraph);
//         for(int n: *res.second)
//             resout << n <<" ";
//         resout<<std::endl;
//         resout<< (res.first / 2.0) <<std::endl;

//         community->clear();
//         community_map->clear();
//         g.clear();
//     }
// }

// std::pair<std::vector<std::vector<int>*>*, std::vector<double>*> groundtruth_dsd_read(std::string dataset){
//     std::ifstream fp;
//     fp.open(dataset+".ground_truth_res");
//     std::string fp_line;
//     auto dsd = new std::vector<std::vector<int>*>();
//     auto groundtruth_density = new std::vector<double>();
//     unsigned int count = 0;
//     while(getline(fp, fp_line)) {
//         std::stringstream ss0(fp_line);
//         std::string temp_str;
//         dsd->push_back(new std::vector<int>());
//         while (std::getline(ss0, temp_str, ' ')){
//             dsd->at(count)->push_back(stoi(temp_str));
//         }
//         getline(fp, fp_line);
//         groundtruth_density->push_back(stod(fp_line));
//         count++;
//     }
//     return {dsd, groundtruth_density};
// }

// double f1score(std::vector<int>* groundtruth, std::vector<int>* revealed){
//     unsigned int tp = 0;
//     for(unsigned int node: * groundtruth){
//         if(std::find(revealed->begin(), revealed->end(), node) != revealed->end())
//             tp++;
//     }

//     double precison = tp * 1.0 / groundtruth->size();
//     double recall = tp * 1.0 / revealed->size();
//     return (2 * precison * recall / (precison + recall));
// }

#endif