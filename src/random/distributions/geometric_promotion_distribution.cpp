#include "geometric_promotion_distribution.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

int GeometricPromotionDistribution::sampleMovesUntilPromotion(
    double uniform_u) const {
  const double p = std::clamp(promotion_probability, 0.000001, 0.999999);

  // Keep U strictly > 0 to avoid ln(0).
  const double min_u = std::numeric_limits<double>::min();
  const double u = std::clamp(uniform_u, min_u, 1.0);

  const double value = std::floor(std::log(u) / std::log(1.0 - p)) + 1.0;
  return std::max(1, static_cast<int>(value));
}
