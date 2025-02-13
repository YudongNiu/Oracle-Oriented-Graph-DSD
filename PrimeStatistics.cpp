#include <iostream>
#include "graph.cpp"
#include "OrderedProbe.cpp"
#include "SAMPLE.cpp"

int main(int argc, char *argv[]) {
    auto dataset = std::string(argv[1]);

    Graph* g;
    if(dataset=="amazon"||dataset=="dblp"||dataset=="youtube"||dataset=="lj"||dataset=="orkut"||dataset=="friendster") g = new Graph(dataset); /* datasets with communities*/
    else g = new Graph(dataset, false); /* signed networks*/

    if(dataset == "amazon" || dataset == "dblp" || dataset == "youtube" || dataset == "orkut" || dataset == "lj" || dataset =="friendster") {
        std::string community_file = "datasets/"+dataset + ".cmty_filter";
        std::ifstream community_in;
        community_in.open(community_file);
        std::string community_line;

        auto community = new std::unordered_map<unsigned int, bool>();

        double avg_prime_edge_num = 0.0;
        double avg_prime_node_num = 0.0;
        double avg_prime_node_edge_ratio = 0.0;

        int CMTY_COUNT = 0;
        while (getline(community_in, community_line)) {
            std::stringstream ss0(community_line);
            std::string temp_str;
            while (std::getline(ss0, temp_str, '\t'))
                community->insert({(unsigned int) stoi(temp_str), true});
            g->setPrime(community);

            avg_prime_edge_num += g->getPrimeEdgeNum();
            avg_prime_node_num += g->getPrimeNodeNum();
            avg_prime_node_edge_ratio += g->getPrimeNodeEdgeRatio();
            
            g->clear();
            community->clear();
            CMTY_COUNT++;
            if(CMTY_COUNT >= 1000) break;
        }

        avg_prime_edge_num /= CMTY_COUNT;
        avg_prime_node_num /= CMTY_COUNT;
        avg_prime_node_edge_ratio /= CMTY_COUNT;

        std::cout<<"avg_prime_node_num: "<<avg_prime_node_num<<std::endl;
        std::cout<<"avg_prime_edge_num: "<<avg_prime_edge_num<<std::endl;
        std::cout<<"avg_prime_node_edge_ratio: "<<avg_prime_node_edge_ratio<<std::endl;
    }
    else{
        // g->getPrimeDSD();

        int avg_prime_edge_num = g->getPrimeEdgeNum();
        int avg_prime_node_num = g->getPrimeNodeNum();

        std::cout<<"avg_prime_node_num: "<<avg_prime_node_num<<std::endl;
        std::cout<<"avg_prime_edge_num: "<<avg_prime_edge_num<<std::endl;
        std::cout<<"avg_prime_node_edge_ratio: "<<g->getPrimeNodeEdgeRatio()<<std::endl;
    }
    return 0;
}