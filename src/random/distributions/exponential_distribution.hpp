#pragma once

#include "random/random_manager.hpp"

class ExponentialDistribution {
public:
  ExponentialDistribution(double lambda = 1.0 / 10.0) : lambda(lambda) {}

  // Inverse transform: X = (-1/lambda) * ln(U) for U in (0, 1]
  double sample(RandomManager const &random_manager) const;

private:
  double lambda;
};
