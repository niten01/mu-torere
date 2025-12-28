#include "mu-torere/Position.hpp"

namespace mutorere {

auto Position::emptyTile() const -> uint8_t {
  auto empty = ~(whiteMask | blackMask);
  for (auto i : std::views::iota(0UL, empty.size())) {
    if (empty.test(i)) {
      return i;
    }
  }

  assert(false && "No empty tile");
}

} // namespace mutorere
