#include "exponential_distribution.hpp"
#include <cmath>

double
ExponentialDistribution::sample(RandomManager const &random_manager) const {
  double u = random_manager.generateUniform();
  // Ensure u is in (0, 1] to avoid log(0)
  if (u <= 0.0) {
    u = 1e-10;
  }
  return (-1.0 / lambda) * std::log(u);
}
