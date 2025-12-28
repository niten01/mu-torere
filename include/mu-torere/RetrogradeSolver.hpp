#pragma once
#include "mu-torere/Position.hpp"
#include "mu-torere/Rules.hpp"
#include "mu-torere/Tablebase.hpp"

namespace mutorere {

class RetrogradeSolver {
public:
  explicit RetrogradeSolver(const Rules& rules)
      : rules_(rules) {
  }

  [[nodiscard]] auto solveFrom(const Position& start) const -> Tablebase ;

private:
  const Rules& rules_;
};

} // namespace mutorere
