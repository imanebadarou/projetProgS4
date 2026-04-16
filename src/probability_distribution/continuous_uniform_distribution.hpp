#pragma once

#include "random.hpp"

class ContinuousUniformDistribution {
public:
  ContinuousUniformDistribution(double min_value = 0.5, double max_value = 2.0)
      : min_value(min_value), max_value(max_value) {}

  double sample(RandomManager const &random_manager) const;

private:
  double min_value;
  double max_value;
};
