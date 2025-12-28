#pragma once
#include "mu-torere/Player.hpp"
#include <bitset>
#include <cassert>
#include <numeric>
#include <ranges>
#include <utility>

namespace mutorere {

struct Position {
  static constexpr auto NumTiles   = 9;
  static constexpr auto RingSize   = NumTiles - 1;
  static constexpr auto CenterTile = NumTiles - 1;
  static_assert(NumTiles % 2 == 1, "Invalid number of tiles");

  std::bitset<NumTiles> whiteMask;
  std::bitset<NumTiles> blackMask;
  Player sideToMove = Player::White;

  friend auto operator==(const Position&, const Position&)
      -> bool = default;

  [[nodiscard]] auto emptyTile() const -> uint8_t;
};

} // namespace mutorere
