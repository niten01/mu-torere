#pragma once
#include "mu-torere/Rules.hpp"

namespace mutorere {

class MuTorereRules final : public Rules {
public:
  [[nodiscard]] auto legalMoves(const Position& position) const
      -> std::vector<Move> override;
  [[nodiscard]] auto apply(const Position& position,
                           const Move& move) const -> Position override;
  [[nodiscard]] auto isTerminal(const Position& position) const
      -> bool override;

  [[nodiscard]] auto initialPosition() const  -> Position override;

private:
  static auto isAdjacent(uint8_t tile1, uint8_t tile2) -> bool;
  static auto centerMoveAllowed(const Position& position, uint8_t from) -> bool;
};

} // namespace mutorere
