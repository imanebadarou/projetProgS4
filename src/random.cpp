#include <random>

double generate_random_double() {
  std::random_device rd{};
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(
      0.0, 1.0); // Distribution uniforme entre 0 et 1
  return dist(gen);
}
