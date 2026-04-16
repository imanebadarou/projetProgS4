#pragma once

#include "game_mode.hpp"

// Gère l'écran de sélection du mode au démarrage de l'application
class ModeSelector {
public:
  ModeSelector();

  bool render();

  GameMode getSelectedMode() const { return selected_mode; }

  int getWhiteElo() const { return white_elo; }
  int getBlackElo() const { return black_elo; }

  void resetSelection();

private:
  GameMode selected_mode = GameMode::NORMAL;
  bool mode_selected = false;
  int white_elo = 1200;
  int black_elo = 1200;
};
