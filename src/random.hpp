#pragma once

class RandomManager {
public:
  // Tirage uniforme dans [0, 1)
  double generateUniform() const;

  // Retourne true si random_01 < p
  bool bernoulliFromRandom(double random_01, double p) const;
};
