#pragma once
#include "Literal.hpp"
#include <memory>
#include <ostream>
#include <vector>
class Clause : public std::vector<Literal> {
public:
  using Ptr = std::shared_ptr<Clause>;
  bool isUnit() const { return size() == 1; }
  void to_ostream(std::ostream &out) const {
    for (size_t i = 0; i < size(); ++i) {
      at(i).to_ostream(out);
      out << ' ';
    }
    out << "0\n";
  }
};