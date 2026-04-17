#include "continuous_uniform_distribution.hpp"

double ContinuousUniformDistribution::sample(
    RandomManager const &random_manager) const {
  const double random_01 = random_manager.generateUniform();
  return min_value + (max_value - min_value) * random_01;
}
