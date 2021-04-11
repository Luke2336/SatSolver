# SAT Solver

> Practice/Real-Life Applications of Computational Algorithms, Spring 2021
>
> 0710006 KE-YU LU

## Build

```sh
make
./yasat [input_file_name]
```

### Environment

- Linux / macOS
- c++14

### Input Format 

DIMACS

```
c comment
p cnf 3 4
1 2 3 0
1 -2 -3 0
-1 2 -3 0
-1 -2 3 0
```

### Output Foramt

Output the satisfiablity and a solution we found!

```
s SATISFIABLE
v 1 2 3 0
```

```
s UNSATISFIABLE
```

## Implementation

1. Store clauses in **Sparse Metrix** using STL vector
2. **Conflict-Driven Clause Learning (CDCL)**
3. **Boolean Constraint Propagation (BCP)**
    - **2-Literal Watching**
4. **Non-Chronological Backtracking**
    - **First Unique Implication Point (1UIP)**
5. Branching Heuristics
    - **Jeroslaw-Wang Score**
    - **Dynamic Heuristic**: Give higher priority to the new clauses.
    - Data Structure: STL set
6. **Luby Restart**



