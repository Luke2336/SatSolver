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
  std::vector<VarState> &getVars() { return Vars; }
  std::vector<Literal> &getTrail() { return Trail; }
  int getLevel() const { return Level; }
  void setLevel(int l) { Level = l; }
  void increaseLevel() { ++Level; }
  void to_ostream(std::ostream &out) const {
    out << "p cnf " << Vars.size() - 1 << ' ' << Clauses.size() << '\n';
    for (auto c : Clauses)
      c->to_ostream(out);
  }
  void attachClause(Clause::Ptr clause) {
    if (clause->size() < 2)
      return;
    for (int i : {0, 1}) {
      Literal literal = clause->at(i);
      Vars[literal.var()].getWatch(literal.sign()).push_back(clause);
    }
  }
  void setLiteralTrue(const Literal &literal, Clause::Ptr antecedent) {
    VarState &Var = Vars[literal.var()];
    Var.setLevel(Level);
    Var.setStatus(literal.getStatus());
    Var.setAntecedent(antecedent);
    Trail.emplace_back(literal);
  }
  Status checkLiteralStatus(const Literal &literal) const {
    auto VarStatus = Vars[literal.var()].getStatus();
    if (VarStatus == Status::Undef)
      return Status::Undef;
    return literal.getStatus() == VarStatus ? Status::True : Status::False;
  }
};