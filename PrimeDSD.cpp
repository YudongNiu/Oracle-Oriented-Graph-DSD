#include <iostream>
#include "graph.cpp"
#include "OrderedProbe.cpp"
#include "SAMPLE.cpp"

int main(int argc, char *argv[]) {
    auto dataset = std::string(argv[1]);

    Graph* g;
    if(dataset=="amazon"||dataset=="dblp"||dataset=="youtube"||dataset=="lj"||dataset=="orkut"||dataset=="friendster") g = new Graph(dataset); /* datasets with communities*/
    else g = new Graph(dataset, false); /* signed networks*/

    g->core();

    if(dataset == "amazon" || dataset == "dblp" || dataset == "youtube" || dataset == "orkut" || dataset == "lj" || dataset =="friendster") {
        std::string community_file = "datasets/"+dataset + ".cmty_sort";
        std::ifstream community_in;
        community_in.open(community_file);
        std::string community_line;

        double avg_prime_dsd = 0.0;
        double avg_suitable_core_edge_ratio = 0.0;

        auto community = new std::unordered_map<unsigned int, bool>();

        int CMTY_COUNT = 0;
        while (getline(community_in, community_line)) {
            std::stringstream ss0(community_line);
            std::string temp_str;
            while (std::getline(ss0, temp_str, '\t'))
                community->insert({(unsigned int) stoi(temp_str), true});
            g->setPrime(community);
            g->getPrimeDSD();

            double suitableCoreEdgeRatio = g->SuitableCoreEdgeRatio();
            std::cout<<"PrimeDSD: "<<g->PrimeDS<<std::endl;
            std::cout<<"SuitableCoreEdgeRatio: "<<suitableCoreEdgeRatio<<std::endl;
            
            avg_prime_dsd += g->PrimeDS;
            avg_suitable_core_edge_ratio += suitableCoreEdgeRatio;

            g->clear();
            community->clear();
            CMTY_COUNT++;
            if(CMTY_COUNT >= 100000) break;
        }

        avg_prime_dsd /= CMTY_COUNT;
        avg_suitable_core_edge_ratio /= CMTY_COUNT;

        std::cout<<"~PrimeDSD: "<<avg_prime_dsd<<std::endl;
        std::cout<<"~SuitableCoreEdgeRatio: "<<avg_suitable_core_edge_ratio<<std::endl;
    }
    else{
        g->getPrimeDSD();
        std::cout<<"~PrimeDSD: "<<g->PrimeDS<<std::endl;
        double suitableCoreEdgeRatio = g->SuitableCoreEdgeRatio();
        std::cout<<"~SuitableCoreEdgeRatio: "<<suitableCoreEdgeRatio<<std::endl;
    }
    return 0;
}