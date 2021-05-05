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

### Milestone 1

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

### Milestone 2

-   Restrict the length of clause learned in CDCL.
    -   The max_length is changing randomly in range $[5, 15)$ when restarting.
-   Fixed some efficiency bugs.

## Result

| Benchmark | Time (s) |
| --------- | ---- |
| aim-100-1_6-no-1.cnf  | 0.003 |
|aim-100-1_6-yes1-1.cnf   | 0.005 |
|aim-200-1_6-no-1.cnf  | 0.005 |
|aim-200-1_6-yes1-1.cnf  | 0.008 |
|aim-50-1_6-no-1.cnf  | 0.003 |
|aim-50-1_6-yes1-1.cnf  | 0.003 |
|dubois100.cnf  | 0.022 |
|dubois20.cnf  | 0.005 |
|ii16a1.cnf  | 0.053 |
|ii32a1.cnf  | 0.025 |
|ii8a1.cnf  | 0.004 |
|jnh1.cnf  | 0.009 |
|jnh10.cnf  | 0.009 |
|jnh11.cnf  | 0.012 |
|par16-1-c.cnf  | 0.008 |
|par16-1.cnf  | 0.014 |
|par32-1-c.cnf  | 0.040 |
|par32-1.cnf  | 0.049 |
|par8-1-c.cnf  | 0.004 |
|par8-1.cnf  | 0.008 |