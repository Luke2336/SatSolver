#include "Parser.h"
#include "Solver.hpp"
#include "SolverContext.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
char *outputfile(char *input) {
  int len = std::strlen(input);
  char *outfilename = new char[len];
  outfilename[0] = '\0';
  strcpy(outfilename, input);
  outfilename[len - 3] = 's';
  outfilename[len - 2] = 'a';
  outfilename[len - 1] = 't';
  return outfilename;
}
int main(int argc, char *argv[]) {
  if (argc <= 1) {
    assert(false && "Argument error!!");
  }
  srand(time(0));
  vector<vector<int>> clauses;
  int maxVarIndex;
  parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
  SolverContext solverContext(clauses, maxVarIndex);
  // solverContext.to_ostream(std::cout);
  Solver solver(solverContext);
  solver.solve();
  freopen(outputfile(argv[1]), "w", stdout);
  solverContext.to_ostream(std::cout);
  return 0;
}