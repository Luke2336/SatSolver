#pragma once
#include "Clause.hpp"
#include "Literal.hpp"
#include "VarState.hpp"
#include <cmath>
#include <ostream>
class SolverContext {
private:
  std::vector<Clause::Ptr> Clauses;
  std::vector<VarState> Vars;
  std::vector<Literal> Trail;
  int Level;

public:
  SolverContext(const std::vector<std::vector<int>> &clauses,
                const int maxVarIndex)
      : Clauses(), Vars(maxVarIndex + 1), Trail(), Level(0) {
    for (const auto &oldC : clauses) {
      Clause newC;
      for (int lit : oldC)
        newC.emplace_back(std::abs(lit), lit < 0);
      Clauses.emplace_back(std::make_shared<Clause>(std::move(newC)));
    }
  }
  std::vector<Clause::Ptr> &getClauses() { return Clauses; }
  std::vector<VarState> getVars() { return Vars; }
  std::vector<Literal> getTrail() { return Trail; }
  int getLevel() const { return Level; }
  void setLevel(int l) { Level = l; }
  void to_ostream(std::ostream &out) const {
    out << "p cnf " << Vars.size() - 1 << ' ' << Clauses.size() << '\n';
    for (auto c : Clauses)
      c->to_ostream(out);
  }
};