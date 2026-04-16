#pragma once

#include "game_mode.hpp"

/**
 * @brief Gestionnaire central pour le mode de jeu actif
 *
 * Permet de sélectionner le mode de jeu et d'y accéder depuis
 * n'importe quel point du code sans passer de paramètres.
 */
class GameModeManager {
public:
  static GameModeManager &getInstance() {
    static GameModeManager instance;
    return instance;
  }

  GameMode getGameMode() const { return current_mode; }
  void setGameMode(GameMode mode) { current_mode = mode; }

  bool isRandomMode() const { return current_mode == GameMode::RANDOM; }
  bool isNormalMode() const { return current_mode == GameMode::NORMAL; }

private:
  GameMode current_mode = GameMode::NORMAL;

  // Constructeur privé pour pattern Singleton
  GameModeManager() = default;
};
