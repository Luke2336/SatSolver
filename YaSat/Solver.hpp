#pragma once
#include "SolverContext.hpp"
#include <algorithm>
class Solver {
private:
  SolverContext &Context;
  int CntConflict;
  const int RestartBase;
  const double RestartInc;
  
  static double luby(double y, int x) {
    int size, seq;
    for (size = 1, seq = 0; size < x + 1; ++seq, size = size * 2 + 1) {
    }
    while (size - 1 != x) {
      size = (size - 1) >> 1;
      --seq;
      x %= size;
    }
    return pow(y, seq);
  }

  Clause::Ptr backtrack(Clause::Ptr ConflictClause) {
    auto &Trail = Context.getTrail();
    if (Context.getLevel() == 0)
      return nullptr;
    int CntLevelVar = 0;
    auto &Vars = Context.getVars();
    Clause TmpClause;
    for (auto lit : *ConflictClause) {
      auto &var = Vars[lit.var()];
      if (var.getLevel() == 0)
        continue;
      var.setMark(true);
      if (var.getLevel() == Context.getLevel())
        ++CntLevelVar;
      else
        TmpClause.emplace_back(lit);
    }
    Literal FUIP;
    for (;;) {
      auto &lit = Trail.back();
      auto &var = Vars[lit.var()];
      Trail.pop_back();
      var.setStatus(Status::Undef);
      if (!var.getMark())
        continue;
      var.setMark(false);
      if ((--CntLevelVar) == 0) {
        FUIP = lit;
        break;
      }
      auto Antecedent = var.getAntecedent();
      if (Antecedent == nullptr)
        continue;
      for (size_t i = 1; i < Antecedent->size(); ++i) {
        auto new_lit = Antecedent->at(i);
        auto &new_var = Vars[new_lit.var()];
        if (new_var.getMark() || new_var.getLevel() == 0)
          continue;
        new_var.setMark(true);
        if (new_var.getLevel() == Context.getLevel())
          ++CntLevelVar;
        else
          TmpClause.push_back(new_lit);
      }
    }
    Clause::Ptr ret = std::make_shared<Clause>();
    ret->emplace_back(~FUIP);
    int MaxLevel = 0;
    for (auto &lit : TmpClause) {
      bool Minimizable = false;
      if (Vars[lit.var()].getAntecedent() != nullptr) {
        Minimizable = true;
        for (auto &l : *Vars[lit.var()].getAntecedent()) {
          if (Vars[l.var()].getMark() != true) {
            Minimizable = false;
            break;
          }
        }
      }
      if (!Minimizable) {
        MaxLevel = std::max(MaxLevel, Vars[lit.var()].getLevel());
        ret->emplace_back(lit);
      }
    }
    for (auto &lit : TmpClause)
      Vars[lit.var()].setMark(false);
    while (Trail.size() && Vars[Trail.back().var()].getLevel() > MaxLevel) {
      Vars[Trail.back().var()].setStatus(Status::Undef);
      Context.addHeap(Trail.back().var());
      Trail.pop_back();
    }
    Context.initHeap();
    Context.setLevel(MaxLevel);
    Context.increasePower();
    Context.attachClause(ret);
    return ret;
  }

  bool unitPropagation(Literal literal) {
    bool Restart;
    Clause::Ptr antecedent = nullptr;
    auto &Trail = Context.getTrail();
    auto &Vars = Context.getVars();
    do {
      Restart = false;
      Context.setLiteralTrue(literal, antecedent);
      for (auto TrailId = Trail.size() - 1; TrailId < Trail.size(); ++TrailId) {
        literal = Trail[TrailId];
        auto &Watch = Vars.at(literal.var()).getWatch((~literal).sign());
        for (size_t WatchId = 0; WatchId < Watch.size(); ++WatchId) {
          auto clause = Watch[WatchId];
          if (clause->at(0) != (~literal))
            std::swap(clause->at(0), clause->at(1));
          Status status = Context.checkLiteralStatus(clause->at(1));
          if (status == Status::True)
            continue;
          size_t LitId;
          for (LitId = 2; LitId < clause->size(); ++LitId) {
            if (Context.checkLiteralStatus(clause->at(LitId)) != Status::False)
              break;
          }
          if (LitId < clause->size()) {
            std::swap(clause->at(LitId), clause->at(0));
            // Remove from Watch List
            Watch[WatchId] = Watch.back();
            Watch.pop_back();
            WatchId--;
            // Add to Watch List
            Vars.at(clause->at(0).var())
                .getWatch(clause->at(0).sign())
                .emplace_back(clause);
          } else if (status == Status::Undef) {
            std::swap(clause->at(0), clause->at(1));
            Context.setLiteralTrue(clause->at(0), clause);
          } else {
            antecedent = backtrack(clause);
            if (antecedent == nullptr)
              return false;
            literal = antecedent->at(0);
            Context.getClauses().emplace_back(antecedent);
            ++CntConflict;
            Restart = true;
            break;
          }
        }
        if (Restart)
          break;
      }
    } while (Restart);
    return true;
  }

  bool simplifyClause(Clause::Ptr C) {
    Clause Tmp = std::move(*C);
    C->clear();
    for (auto lit : Tmp) {
      auto status = Context.checkLiteralStatus(lit);
      if (status == Status::True)
        return true;
      if (status == Status::False)
        continue;
      C->emplace_back(lit);
    }
    return false;
  }

  Status solve(int ConflictMax) {
    Context.setLevel(0);
    Context.initHeap();
    for (auto clause : Context.getClauses()) {
      if (simplifyClause(clause))
        continue;
      if (clause->empty())
        return Status::False;
      if (clause->isUnit()) {
        if (!unitPropagation(clause->at(0)))
          return Status::False;
      } else
        Context.attachClause(clause);
    }
    for (;;) {
      Literal literal = Context.selectLiteral();
      if (literal.isNull())
        break;
      Context.increaseLevel();
      if (!unitPropagation(literal))
        return Status::False;
      if (CntConflict > ConflictMax)
        return Status::Undef;
    }
    return Status::True;
  }

public:
  Solver(SolverContext &Context)
      : Context(Context), CntConflict(0), RestartBase(100), RestartInc(2) {}
  void solve() {
    for (int CntRestart = 0;; ++CntRestart) {
      Status status = solve(RestartBase * luby(RestartInc, CntRestart));
      if (status != Status::Undef) {
        Context.setSAT(status == Status::True);
        return;
      }
      Context.init();
      CntConflict = 0;
    }
  }
};