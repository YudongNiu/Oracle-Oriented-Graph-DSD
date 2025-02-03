# LLM-curated Graph for Densest Subgraph Discovery

Preparation
-------
First, compile the code for LLM-curated-Graph-DSD by executing the following three commands on linux, corresponding to the deterministic approach, AAS algorithm and HAAS algorithm respectively:

```sh
g++ -O3 deterministic.cpp -o deterministic
g++ -O3 aas.cpp -o aas
g++ -O3 haas.cpp -o haas
```

Second, before running the deterministic approach, please generate the sorted edge list for corresponding preorder heuristics following the steps below.

(1) Compile the main_probe_order.cpp using the following command
```sh
g++ -O3 main_probe_order.cpp -o probe_order
```
(2) generate the sorted list of edges in the raw data graph according to certain preorder heuristics using the following command
```sh
./probe_order dataset heuristic
```

Parameter "dataset" is the name of the raw data graph;
Parameter "heuristic" is the preorder heuristic that will be employed in the deterministic approach. Six different heuristics are implemented in the current version: "degree", "triangle", "core", "truss", "ld" and "soe".

Third, the CoreExact method implemented in https://github.com/TalionS/DensestSubgraph is employed in our project for computing the maximum subgraph density of the remaining subgraph in each iteration in deterministic approach. Therefore, please download and compile the project and include the generated executable file within your workspace.

Running code
-------
After preparation, we can execute the deterministic approach using the following command on linux

```sh
./deterministic dataset heuristic
```

The following two commands will execute the sampling-based methods AAS and HAAS respectively

```sh
./aas dataset epsilon p0
./haas dataset epsilon p0
```
Parameter "dataset" is the name of the raw data graph; 
Parameter "epsilon" is the required approximation ratio;
Parameter "p0" is the initial sampling probability.
