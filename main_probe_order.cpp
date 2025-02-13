#include "ProbeStrategies.cpp"

int main(int argc, char *argv[]){
    auto dataset = std::string(argv[1]);
    auto method = std::string(argv[2]);

    Graph *g;
    if(dataset=="amazon"||dataset=="dblp"||dataset=="youtube"||dataset=="lj"||dataset=="orkut"||dataset=="friendster") g = new Graph(dataset);
    else g = new Graph(dataset, false);

    std::string order_res_file = "datasets/"+dataset + "."+method+"_order";
    std::ofstream order_out;
    order_out.open(order_res_file);

    if(method == "wedge"){
        degsum_based_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "triangle"){
        triangle_based_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "core"){
        core_based_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "wedge_static"){
        degsum_static_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "triangle_static"){
        triangle_static_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "core_static"){
        core_static_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    else if(method == "truss_static"){
        truss_static_probe baseline(g);
        baseline.run();
        for(unsigned int i=0;i<baseline.order0.size();i++)
            order_out<<baseline.order0[i]<<" "<<baseline.order1[i]<<std::endl;
    }
    order_out.close();
}