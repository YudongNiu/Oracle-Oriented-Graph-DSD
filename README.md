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

To run the code for corresponding algorithms, execute the following command on linux:

```sh
./deterministic dataset heuristic
```

```sh
./aas dataset epsilon p0
```

```sh
./haas dataset epsilon p0
```
