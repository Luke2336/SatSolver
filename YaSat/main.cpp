#include "Parser.h"
#include "SolverContext.hpp"
#include "Solver.hpp"
#include <cassert>
#include <iostream>
int main(int argc, char *argv[]) {
  if (argc <= 1) {
    assert(false && "Argument error!!");
  }
  vector<vector<int>> clauses;
  int maxVarIndex;
  parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
  SolverContext solverContext(clauses, maxVarIndex);
  solverContext.to_ostream(std::cout);
  Solver solver(solverContext);
  bool SAT = solver.solve();
  std::cout << (SAT ? "SAT" : "UNSAT") << '\n';
  return 0;
}