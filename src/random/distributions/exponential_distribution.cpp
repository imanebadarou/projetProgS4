#include "exponential_distribution.hpp"
#include <cmath>

double
ExponentialDistribution::sample(RandomManager const &random_manager) const {
  double u = random_manager.generateUniform();
  // Garantit que u est dans (0, 1] pour eviter log(0)
  if (u <= 0.0) {
    u = 1e-10;
  }
  return (-1.0 / lambda) * std::log(u);
}
