#pragma once
#include "mu-torere/Outcome.hpp"
#include "mu-torere/Position.hpp"
#include <unordered_map>

namespace mutorere {

struct PositionHasher {
  auto operator()(const Position& position) const -> std::size_t;
};

class Tablebase {
public:
  auto lookup(const Position& position) const -> std::optional<Outcome>;
  auto store(const Position& position, const Outcome& outcome) -> void;
  auto size() const -> size_t {
    return db_.size();
  }

private:
  std::unordered_map<Position, Outcome, PositionHasher> db_;
};

} // namespace mutorere
