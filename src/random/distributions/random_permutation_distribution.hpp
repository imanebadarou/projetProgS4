#pragma once

#include "random/random_manager.hpp"

#include <array>

class RandomPermutationDistribution {
public:
  // Returns a shuffled encoding of [R, N, B, Q, K, B, N, R] using Fisher-Yates.
  std::array<int, 8>
  sampleBackRankPermutation(RandomManager const &random_manager) const;
};
