#include "mu-torere/Tablebase.hpp"
#include "mu-torere/Player.hpp"

namespace mutorere {

namespace {
// Source - https://stackoverflow.com/a
// Posted by Scott Howlett, modified by community. See post 'Timeline' for
// change history Retrieved 2025-12-27, License - CC BY-SA 3.0
template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
  std::hash<T> hasher;
  const std::size_t kMul = 0x9ddfea08eb382d69ULL;
  std::size_t a          = (hasher(v) ^ seed) * kMul;
  a ^= (a >> 47U);
  std::size_t b = (seed ^ a) * kMul;
  b ^= (b >> 47U);
  seed = b * kMul;
}
} // namespace

auto PositionHasher::operator()(const Position& position) const
    -> std::size_t {
  std::size_t seed = std::hash<Player>{}(position.sideToMove);
  hash_combine(seed, position.whiteMask);
  hash_combine(seed, position.blackMask);
  return seed;
}

auto Tablebase::lookup(const Position& position) const
    -> std::optional<Outcome> {
  if (!db_.contains(position)) {
    return std::nullopt;
  }

  return db_.at(position);
}

auto Tablebase::store(const Position& position, const Outcome& outcome)
    -> void {
  db_[position] = outcome;
}

} // namespace mutorere
