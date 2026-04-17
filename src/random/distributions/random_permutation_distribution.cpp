#include "random_permutation_distribution.hpp"

#include <algorithm>

std::array<int, 8> RandomPermutationDistribution::sampleBackRankPermutation(
    RandomManager const &random_manager) const {
  std::array<int, 8> permutation = {
      0, // rook
      1, // knight
      2, // bishop
      3, // queen
      4, // king
      2, // bishop
      1, // knight
      0  // rook
  };

  // Fisher-Yates shuffle: for i from n-1 to 1, swap(i, random[0..i]).
  for (int i = 7; i > 0; --i) {
    size_t j = static_cast<size_t>(random_manager.generateUniform() * (i + 1));
    if (j > static_cast<size_t>(i)) {
      j = static_cast<size_t>(i);
    }
    std::swap(permutation[static_cast<size_t>(i)], permutation[j]);
  }

  return permutation;
}
