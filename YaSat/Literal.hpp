#pragma once
#include "Util.hpp"
#include <ostream>
class Literal {
private:
  int v;

public:
  Literal() : v(0) {}
  Literal(int var, bool sign) : v((var << 1) | (int)sign) {}
  Literal operator^(bool b) const {
    Literal ret;
    ret.v = v ^ (int)b;
    return ret;
  }
  Literal operator~() const { return operator^(1); }
  bool sign() const { return v & 1; }
  Status getStatus() const { return sign() ? Status::False : Status::True; }
  int var() const { return v >> 1; }
  operator int() const { return v; }
  void to_ostream(std::ostream &out) const {
    out << (sign() ? "-" : "") << var();
  }
  bool isNull() { return v == 0; }
};