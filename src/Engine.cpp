#include "mu-torere/Engine.hpp"
#include "mu-torere/Outcome.hpp"
#include <algorithm>

namespace mutorere {

namespace {

constexpr auto invert(Result result) -> Result {
  if (result == Result::Win) {
    return Result::Loss;
  }
  if (result == Result::Loss) {
    return Result::Win;
  }
  return Result::Unknown;
}

} // namespace

Engine::Engine(const Rules& rules, Tablebase tb)
    : rules_{ rules },
      tb_{ std::move(tb) } {
}

auto Engine::bestMove(const Position& position) const
    -> std::optional<Move> {
  auto moves = rules_.legalMoves(position);
  if (moves.empty()) {
    return std::nullopt;
  }

  return *std::ranges::max_element(
      moves, [&](const Move& a, const Move& b) {
        return moveScore(position, a) < moveScore(position, b);
      });
}

auto Engine::moveScore(const Position& position, const Move& move) const
    -> std::pair<int, int> {
  Position next = rules_.apply(position, move);
  auto outcome  = tb_.lookup(next).value_or(
      Outcome{ .result = Result::Unknown, .remoteness = 0 });

  Result fromCurrent = invert(outcome.result);
  int tier           = 0;
  int tie            = 0;
  if (fromCurrent == Result::Win) {
    tier = 2;
    tie  = -outcome.remoteness;
  } else if (fromCurrent == Result::Unknown) {
    tier = 1;
    tie  = 0;
  } else if (fromCurrent == Result::Loss) {
    tier = 0;
    tie  = outcome.remoteness;
  }

  return std::make_pair(tier, tie);
}

} // namespace mutorere
