#pragma once

#include "game_mode.hpp"

/**
 * @brief Interface de sélection du mode de jeu
 *
 * Gère l'écran de sélection du mode au démarrage de l'application
 */
class ModeSelector {
public:
  ModeSelector();

  /**
   * @brief Rend l'interface de sélection du mode
   * @return true si un mode a été sélectionné et prêt à démarrer
   */
  bool render();

  /**
   * @brief Obtient le mode sélectionné
   */
  GameMode getSelectedMode() const { return selected_mode; }

  /**
   * @brief Réinitialise l'état de sélection
   */
  void resetSelection();

private:
  GameMode selected_mode = GameMode::NORMAL;
  bool mode_selected = false;
};
