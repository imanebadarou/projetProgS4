#include "random_manager.hpp"

#include <random>

double RandomManager::generateUniform() const {
  std::uniform_real_distribution<> dist(0.0, 1.0);
  static std::mt19937 gen{std::random_device{}()};
  return dist(gen);
}

bool RandomManager::bernoulliFromRandom(double random_01, double p) const {
  return random_01 < p;
}
