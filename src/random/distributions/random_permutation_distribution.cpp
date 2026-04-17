#include "random_permutation_distribution.hpp"

#include <algorithm>

std::array<int, 8> RandomPermutationDistribution::sampleBackRankPermutation(
    RandomManager const &random_manager) const {
  std::array<int, 8> permutation = {
      0, // tour
      1, // cavalier
      2, // fou
      3, // reine
      4, // roi
      2, // fou
      1, // cavalier
      0  // tour
  };

  // Melange de Fisher-Yates : pour i de n-1 a 1, echanger(i, random[0..i]).
  for (int i = 7; i > 0; --i) {
    size_t j = static_cast<size_t>(random_manager.generateUniform() * (i + 1));
    if (j > static_cast<size_t>(i)) {
      j = static_cast<size_t>(i);
    }
    std::swap(permutation[static_cast<size_t>(i)], permutation[j]);
  }

  return permutation;
}
