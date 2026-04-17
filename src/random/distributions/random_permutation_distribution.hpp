#pragma once

#include "random/random_manager.hpp"

#include <array>

class RandomPermutationDistribution {
public:
  // Retourne un encodage melange de [R, N, B, Q, K, B, N, R] via Fisher-Yates.
  std::array<int, 8>
  sampleBackRankPermutation(RandomManager const &random_manager) const;
};
