#pragma once
#include <cstdint>

namespace mutorere {

enum class Result : uint8_t {
  Unknown,
  Win,
  Loss,
};

struct Outcome {
  Result result  = Result::Unknown;
  int remoteness = 0;
};

} // namespace mutorere
