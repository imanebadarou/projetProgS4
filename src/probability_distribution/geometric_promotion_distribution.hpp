#pragma once

class GeometricPromotionDistribution {
public:
  explicit GeometricPromotionDistribution(double probability = 0.05)
      : promotion_probability(probability) {}

  // Inverse CDF for geometric law (support starts at 1):
  // X = floor(ln(U) / ln(1 - p)) + 1 with U in (0, 1].
  int sampleMovesUntilPromotion(double uniform_u) const;

private:
  double promotion_probability;
};
