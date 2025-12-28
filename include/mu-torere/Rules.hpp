#pragma once
#include "mu-torere/Move.hpp"
#include "mu-torere/Position.hpp"
#include <vector>

namespace mutorere {

struct Rules {
  virtual ~Rules() = default;

  [[nodiscard]] virtual auto legalMoves(const Position& position) const
      -> std::vector<Move> = 0;

  [[nodiscard]] virtual auto apply(const Position& position,
                                   const Move& move) const
      -> Position = 0;

  [[nodiscard]] virtual auto isTerminal(const Position& position) const
      -> bool = 0;

  [[nodiscard]] virtual auto initialPosition() const -> Position = 0;
};

} // namespace mutorere
