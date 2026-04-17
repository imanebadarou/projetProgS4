#pragma once

class GeometricPromotionDistribution {
public:
  explicit GeometricPromotionDistribution(double probability = 0.05)
      : promotion_probability(probability) {}

  // CDF inverse pour la loi geometrique (le support commence a 1) :
  // X = floor(ln(U) / ln(1 - p)) + 1 avec U dans (0, 1].
  int sampleMovesUntilPromotion(double uniform_u) const;

private:
  double promotion_probability;
};
