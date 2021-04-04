#pragma once
#include "Literal.hpp"
#include <memory>
#include <vector>
class Clause : public std::vector<Literal> {
public:
  using Ptr = std::shared_ptr<Clause>;
};