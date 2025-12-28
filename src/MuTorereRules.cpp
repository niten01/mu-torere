#include "mu-torere/MuTorereRules.hpp"
#include "mu-torere/Position.hpp"

namespace mutorere {

namespace {

auto currentMasks(const Position& position) {
  return position.sideToMove == Player::White
             ? std::make_pair(position.whiteMask, position.blackMask)
             : std::make_pair(position.blackMask, position.whiteMask);
}

auto setCurrentMasks(Position& position, auto current, auto other) {
  if (position.sideToMove == Player::White) {
    position.whiteMask = current;
    position.blackMask = other;
  } else {
    position.blackMask = current;
    position.whiteMask = other;
  }
}

auto adjacentOnRing(uint8_t tile) {
  constexpr auto RingSize = Position::NumTiles - 1;
  return std::make_pair((tile + 1) % RingSize,
                        (tile + RingSize - 1) % RingSize);
}

} // namespace

auto MuTorereRules::legalMoves(const Position& position) const
    -> std::vector<Move> {
  std::vector<Move> res;

  auto empty            = position.emptyTile();
  auto [current, other] = currentMasks(position);

  for (auto from : std::views::iota(0UL, current.size())) {
    if (!current.test(from)) {
      continue;
    }

    if (!isAdjacent(from, empty)) {
      continue;
    }

    if (empty == Position::CenterTile &&
        !centerMoveAllowed(position, from)) {
      continue;
    }

    res.push_back(Move{ static_cast<uint8_t>(from) });
  }

  return res;
}

auto MuTorereRules::apply(const Position& position,
                          const Move& move) const -> Position {
  auto [current, other] = currentMasks(position);
  current.reset(move.from);
  current.set(position.emptyTile());

  auto res = position;
  setCurrentMasks(res, current, other);
  res.sideToMove = position.sideToMove == Player::White ? Player::Black
                                                        : Player::White;

  return res;
}

auto MuTorereRules::isTerminal(const Position& position) const -> bool {
  return legalMoves(position).empty();
}

auto MuTorereRules::isAdjacent(uint8_t tile1, uint8_t tile2) -> bool {
  if (tile1 == tile2) {
    return false;
  }

  if (tile1 == Position::CenterTile || tile2 == Position::CenterTile) {
    return true;
  }

  auto [l, r] = adjacentOnRing(tile1);
  return tile2 == l || tile2 == r;
}

auto MuTorereRules::centerMoveAllowed(const Position& position,
                                      uint8_t from) -> bool {
  auto [_, other] = currentMasks(position);
  assert(position.emptyTile() == Position::CenterTile &&
         "Center move not available");

  auto [l, r] = adjacentOnRing(from);
  return other.test(l) || other.test(r);
}

auto MuTorereRules::initialPosition() const -> Position {
  Position res;
  res.sideToMove = Player::White;
  for (int i = 0; i < Position::RingSize / 2; i++) {
    res.whiteMask.set(i);
  }
  for (int i = (Position::RingSize / 2); i < Position::RingSize; i++) {
    res.blackMask.set(i);
  }
  return res;
}

} // namespace mutorere
