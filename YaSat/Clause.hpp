#pragma once
#include "Literal.hpp"
#include <memory>
#include <ostream>
#include <vector>
class Clause : public std::vector<Literal> {
public:
  using Ptr = std::shared_ptr<Clause>;
  void to_ostream(std::ostream &out) const {
    for (int i = 0; i < size(); ++i) {
      at(i).to_ostream(out);
      out << ' ';
    }
    out << "0\n";
  }
};