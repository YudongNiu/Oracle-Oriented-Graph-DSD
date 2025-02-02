# LLM-curated Graph for Densest Subgraph Discovery

Compile
-------
Please compile the code for LLM-curated-Graph-DSD by executing the following three commands on linux, corresponding to the deterministic approach, AAS algorithm and HAAS algorithm respectively:

```sh
g++ -O3 deterministic.cpp -o deterministic
g++ -O3 aas.cpp -o aas
g++ -O3 haas.cpp -o haas
```

Running code
-------

Before running the deterministic approach, please first compile the main_probe_order.cpp using the following command
```sh
g++ -O3 main_probe_order.cpp -o probe_order
```
and generate sorted list of edges in the data graph according to certain preorder heuristics
```sh
./probe_order dataset heuristic
```


```sh
./deterministic dataset heuristic
```

Parameter "dataset" is the name of the raw data graph; 
Parameter "heuristic" is the preorder heuristic employed in the deterministic approach. 
Six different heuristics are implemented in the current version: degree, triangle, core, truss, ld, soe.

```sh
./aas dataset epsilon p0
./haas dataset epsilon p0
```
Parameter "dataset" is the name of the raw data graph; 
Parameter "epsilon" is the required approximation ratio;
Parameter "p0" is the initial sampling probability.
