#include "random.hpp"

#include <random>

double RandomManager::generateUniform() const {
  std::random_device rd{};
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0.0, 1.0);
  return dist(gen);
}

bool RandomManager::bernoulliFromRandom(double random_01, double p) const {
  return random_01 < p;
}
