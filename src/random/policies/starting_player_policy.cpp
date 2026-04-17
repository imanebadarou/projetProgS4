#include "starting_player_policy.hpp"

#include <algorithm>

double
StartingPlayerPolicy::computeLowerEloStartProbability(double elo_white,
                                                      double elo_black) const {
  const double safe_white = std::max(1.0, elo_white);
  const double safe_black = std::max(1.0, elo_black);
  const double lower = std::min(safe_white, safe_black);
  const double higher = std::max(safe_white, safe_black);
  const double ratio = lower / higher;
  const double p = 0.5 + ratio / 2.0;
  return std::clamp(p, 0.0, 1.0);
}

bool StartingPlayerPolicy::shouldWhiteStart(double elo_white, double elo_black,
                                            bool bernoulli_result) const {
  if (elo_white == elo_black) {
    return bernoulli_result;
  }

  const bool lower_starts = bernoulli_result;
  const bool white_is_lower = elo_white < elo_black;
  return lower_starts ? white_is_lower : !white_is_lower;
}