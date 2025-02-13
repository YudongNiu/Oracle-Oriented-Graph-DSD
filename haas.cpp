#include <iostream>
#include "graph.cpp"
#include "OrderedProbe.cpp"
#include "SAMPLE.cpp"

int main(int argc, char *argv[]) {
    int COUNT = 10;

    auto dataset = std::string(argv[1]);
    auto approx_ratio = std::stod(argv[2]);
    auto Oconstant = std::stod(argv[3]);
    auto alpha = std::stod(argv[4]);

    Graph* g;
    if(dataset=="amazon"||dataset=="dblp"||dataset=="youtube"||dataset=="lj"||dataset=="orkut"||dataset=="friendster") g = new Graph(dataset); /* datasets with communities*/
    else g = new Graph(dataset, false); /* signed networks*/

    g->core();

    if(dataset == "amazon" || dataset == "dblp" || dataset == "youtube" || dataset == "orkut" || dataset == "lj" || dataset =="friendster") {
        std::string community_file = "datasets/"+dataset + ".cmty_filter";
        std::ifstream community_in;
        community_in.open(community_file);
        std::string community_line;

        auto community = new std::unordered_map<unsigned int, bool>();

        double avg_probe_ratio = 0.0;
        double avg_min_probe_ratio = 0.0;
        double avg_max_probe_ratio = 0.0;

        double avg_precision = 0.0;
        double avg_min_precision = 0.0;
        double avg_max_precision = 0.0;
        double avg_running_time = 0.0;

        double avg_number_of_iterations = 0.0;

        int CMTY_COUNT = 0;
        while (getline(community_in, community_line)) {
            std::stringstream ss0(community_line);
            std::string temp_str;
            while (std::getline(ss0, temp_str, '\t'))
                community->insert({(unsigned int) stoi(temp_str), true});
            g->setPrime(community);
            g->getPrimeDSD();
            sample_verification baseline(g);

            double precision = 0.0, min_precision = 1.0, max_precision = 0.0;
            double probe_ratio = 0.0, min_probe_ratio = 1.0, max_probe_ratio = 0.0;
            double running_time = 0.0;
            double number_of_iterations = 0.0;
            auto start = std::chrono::steady_clock::now();
            for(unsigned int sample_count = 0;sample_count < COUNT;sample_count++){
                auto res = baseline.verif_cost_sample_induced_lb_stacked_prob(approx_ratio, Oconstant, alpha);
                
                if(res.first.first < min_probe_ratio) min_probe_ratio = res.first.first;
                if(res.first.first > max_probe_ratio) max_probe_ratio = res.first.first;
                probe_ratio += res.first.first;
                if(res.first.second < min_precision) min_precision = res.first.second;
                if(res.first.second > max_precision) max_precision = res.first.second;
                precision += res.first.second;

                number_of_iterations += res.second;
                
                std::cout<<"probe_ratio: "<<res.first.first<<std::endl;
                std::cout<<"precision: "<<res.first.second<<std::endl;
                std::cout<<"num_iterations: "<<res.second<<std::endl;
            }
            auto end = std::chrono::steady_clock::now();

            running_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            running_time /= 1000000000;
            probe_ratio /= COUNT;
            precision /= COUNT;
            running_time /= COUNT;
            number_of_iterations /= COUNT;
            std::cout<<"*max_probe_ratio: "<<max_probe_ratio<<std::endl;
            std::cout<<"*min_probe_ratio: "<<min_probe_ratio<<std::endl;
            std::cout<<"*max_precision: "<<max_precision<<std::endl;
            std::cout<<"*min_precision: "<<min_precision<<std::endl;
            std::cout<<"*probe_ratio: "<<probe_ratio<<std::endl;
            std::cout<<"*precision: "<<precision<<std::endl;
            std::cout<<"*time: "<<running_time<<std::endl;
            std::cout<<"*num_iterations: "<<number_of_iterations<<std::endl;

            avg_precision += precision;
            avg_max_precision += max_precision;
            avg_min_precision += min_precision;

            avg_probe_ratio += probe_ratio;
            avg_max_probe_ratio += max_probe_ratio;
            avg_min_probe_ratio += min_probe_ratio;
            avg_running_time += running_time;

            avg_number_of_iterations += number_of_iterations;
            
            g->clear();
            community->clear();
            CMTY_COUNT++;
            if(CMTY_COUNT >= 1000) break;
        }

        avg_precision /= CMTY_COUNT;
        avg_max_precision /= CMTY_COUNT;
        avg_min_precision /= CMTY_COUNT;

        avg_probe_ratio /= CMTY_COUNT;
        avg_max_probe_ratio /= CMTY_COUNT;
        avg_min_probe_ratio /= CMTY_COUNT;
        avg_running_time /= CMTY_COUNT;
        avg_number_of_iterations /= CMTY_COUNT;

        std::cout<<"~max_probe_ratio: "<<avg_max_probe_ratio<<std::endl;
        std::cout<<"~min_probe_ratio: "<<avg_min_probe_ratio<<std::endl;
        std::cout<<"~max_precision: "<<avg_max_precision<<std::endl;
        std::cout<<"~min_precision: "<<avg_min_precision<<std::endl;
        std::cout<<"~probe_ratio: "<<avg_probe_ratio<<std::endl;
        std::cout<<"~precision: "<<avg_precision<<std::endl;
        std::cout<<"~time: "<<avg_running_time<<std::endl;
        std::cout<<"~num_iterations:"<<avg_number_of_iterations<<std::endl;
    }
    else{
        g->getPrimeDSD();
        sample_verification baseline(g);
        
        double precision = 0.0, min_precision = 1.0, max_precision = 0.0;
        double probe_ratio = 0.0, min_probe_ratio = 1.0, max_probe_ratio = 0.0;
        double running_time = 0.0;
        double number_of_iterations = 0.0;

        auto start = std::chrono::steady_clock::now();
        for(unsigned int sample_count = 0;sample_count < COUNT;sample_count++){
            auto res = baseline.verif_cost_sample_induced_lb_stacked_prob(approx_ratio, Oconstant, alpha);
                
            if(res.first.first < min_probe_ratio) min_probe_ratio = res.first.first;
            if(res.first.first > max_probe_ratio) max_probe_ratio = res.first.first;
            probe_ratio += res.first.first;

            if(res.first.second < min_precision) min_precision = res.first.second;
            if(res.first.second > max_precision) max_precision = res.first.second;
            precision += res.first.second;

            number_of_iterations += res.second;

            std::cout<<"probe_ratio: "<<res.first.first<<std::endl;
            std::cout<<"precision: "<<res.first.second<<std::endl;
            std::cout<<"num_iterations: "<<res.second<<std::endl;
        }
        auto end = std::chrono::steady_clock::now();
        running_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        running_time /= 1000000000;
        running_time /= COUNT;
        probe_ratio /= COUNT;
        precision /= COUNT;
        number_of_iterations /= COUNT;
        std::cout<<"~max_probe_ratio: "<<max_probe_ratio<<std::endl;
        std::cout<<"~min_probe_ratio: "<<min_probe_ratio<<std::endl;
        std::cout<<"~max_precision: "<<max_precision<<std::endl;
        std::cout<<"~min_precision: "<<min_precision<<std::endl;
        std::cout<<"~probe_ratio: "<<probe_ratio<<std::endl;
        std::cout<<"~precision: "<<precision<<std::endl;
        std::cout<<"~time: "<<running_time<<std::endl;
        std::cout<<"~num_iterations: "<<number_of_iterations<<std::endl;
    }
    return 0;
}