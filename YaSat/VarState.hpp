#pragma once
#include "Clause.hpp"
#include "Util.hpp"
#include <vector>

class VarState {
private:
  Status status;
  int level;
  bool mark;
  std::vector<Clause::Ptr> watch[2];
  Clause::Ptr antecedent;
  double score[2];

public:
  VarState()
      : status(Status::Undef), level(0), mark(false), antecedent(nullptr) {
    score[0] = score[1] = 0;
  }
  Status getStatus() const { return status; }
  int getLevel() const { return level; }
  bool getMark() const { return mark; }
  const Clause::Ptr getAntecedent() const { return antecedent; }
  Clause::Ptr getAntecedent() { return antecedent; }
  const std::vector<Clause::Ptr> &getWatch(bool b) const { return watch[b]; }
  std::vector<Clause::Ptr> &getWatch(bool b) { return watch[b]; }
  void setStatus(Status s) { status = s; }
  void setLevel(int l) { level = l; }
  void setMark(bool m) { mark = m; }
  void setAntecedent(Clause::Ptr a) { antecedent = a; }
  double getScore(bool i) const { return score[i]; }
  double &getScore(bool i) { return score[i]; }
  double getScoreSum() const { return score[0] + score[1]; }
};