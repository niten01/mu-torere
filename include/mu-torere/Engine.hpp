#pragma once
#include "mu-torere/Move.hpp"
#include "mu-torere/Rules.hpp"
#include "mu-torere/Tablebase.hpp"

namespace mutorere {

class Engine {
public:
  Engine(const Rules& rules, Tablebase tb);

  auto bestMove(const Position& position) const -> std::optional<Move>;

private:
  auto moveScore(const Position& position, const Move& move) const
      -> std::pair<int, int>;

  const Rules& rules_;
  Tablebase tb_;
};

} // namespace mutorere
