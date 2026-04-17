#pragma once

class StartingPlayerPolicy {
public:
  // p = 0.5 + ratio/2, ratio = min(ELO1, ELO2) / max(ELO1, ELO2)
  double computeLowerEloStartProbability(double elo_white,
                                         double elo_black) const;

  // Retourne true si les blancs commencent.
  bool shouldWhiteStart(double elo_white, double elo_black,
                        bool bernoulli_result) const;
};