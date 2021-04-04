#include "parser.h"
#include <cassert>
int main(int argc, char *argv[]) {
  if (argc <= 1) {
    assert(false && "Argument error!!");
  }
  vector<vector<int>> clauses;
  int maxVarIndex;
  parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
  return 0;
}