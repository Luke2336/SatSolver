#pragma once
#include "Clause.hpp"
#include "Literal.hpp"
#include "VarState.hpp"
#include <cmath>
#include <ostream>
#include <set>
#include <utility>
class SolverContext {
private:
  std::vector<Clause::Ptr> Clauses;
  std::vector<VarState> Vars;
  std::vector<Literal> Trail;
  int Level;
  std::set<std::pair<double, int>> ScoreHeap;
  double ScorePower;
  const double ScorePowerMax;
  const double ScorePowerTimes;

public:
  SolverContext(const std::vector<std::vector<int>> &clauses,
                const int maxVarIndex)
      : Clauses(), Vars(maxVarIndex + 1), Trail(), Level(0), ScorePower(1),
        ScorePowerMax(1e20), ScorePowerTimes(1.001) {
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
  void increasePower() { ScorePower *= ScorePowerTimes; }
  void initHeap() {
    ScoreHeap.clear();
    for (size_t i = 1; i < Vars.size(); ++i) {
      auto &var = Vars[i];
      if (var.getStatus() == Status::Undef)
        ScoreHeap.emplace(var.getScoreSum(), i);
    }
  }
  void rebuildHeap() {
    if (ScorePower < ScorePowerMax)
      return;
    ScorePower = 1;
    for (size_t i = 1; i < Vars.size(); ++i) {
      Vars[i].getScore(0) /= ScorePowerMax;
      Vars[i].getScore(1) /= ScorePowerMax;
    }
    initHeap();
  }
  void removeHeap(size_t vid) {
    auto &var = Vars[vid];
    ScoreHeap.erase(std::make_pair(var.getScoreSum(), vid));
  }
  void addHeap(size_t vid) { ScoreHeap.emplace(Vars[vid].getScoreSum(), vid); }
  Literal selectLiteral() {
    while (ScoreHeap.size()) {
      size_t vid = ScoreHeap.rbegin()->second;
      auto &var = Vars[vid];
      if (var.getStatus() == Status::Undef)
        return Literal(vid, var.getScore(0) < var.getScore(1));
      ScoreHeap.erase(*ScoreHeap.rbegin());
    }
    return Literal(0, false);
  }
  void attachClause(Clause::Ptr clause) {
    if (clause->size() < 2)
      return;
    for (int i : {0, 1}) {
      Literal literal = clause->at(i);
      Vars[literal.var()].getWatch(literal.sign()).emplace_back(clause);
    }
    rebuildHeap();
    for (auto &lit : *clause) {
      removeHeap(lit.var());
      double &score = Vars[lit.var()].getScore(lit.sign());
      score += std::pow(2, -clause->size()) * ScorePower;
      addHeap(lit.var());
    }
  }
  void setLiteralTrue(const Literal &literal, Clause::Ptr antecedent) {
    VarState &Var = Vars[literal.var()];
    removeHeap(literal.var());
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