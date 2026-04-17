#pragma once

#include "board.hpp"
#include "game_mode.hpp"
#include "pieces/piece.hpp"
#include "random/distributions/continuous_uniform_distribution.hpp"
#include "random/distributions/exponential_distribution.hpp"
#include "random/distributions/geometric_promotion_distribution.hpp"
#include "random/distributions/random_permutation_distribution.hpp"
#include "random/random_manager.hpp"
#include <optional>
#include <string>

class GameLogic {
public:
  GameLogic();

  // Execute un coup et met a jour l'etat de la partie
  bool makeMove(coords from, coords to);

  // Recupere le nom de la piece a partir d'un pointeur de piece
  static std::string getPieceName(Piece const *piece);

  // Accesseurs
  bool isGameOver() const { return winner != 0; }
  int getWinner() const { return winner; }
  Color getCurrentTurn() const { return current_turn; }
  Board &getBoard() { return board; }
  GameMode getGameMode() const;
  std::optional<coords> getLastRandomPromotionPos() const {
    return last_promotion_pos;
  }
  double getLastPromotionTime() const { return last_promotion_time; }
  bool hasRandomPromotionOccurred() const {
    return last_promotion_pos.has_value();
  }
  double sampleMoveSpeedFactor() const;
  bool hasMeteoriteEvent() const { return meteor_position.has_value(); }
  std::optional<coords> getMeteoritePosition() const { return meteor_position; }
  double getMeteoriteStartTime() const { return meteor_start_time; }
  void updateMeteoriteEvents();

  // Mutateurs
  void setCurrentTurn(Color color) { current_turn = color; }
  void setGameMode(GameMode mode);
  void resetGame();

private:
  Board board;
  Color current_turn;
  int winner; // 0 = en cours, 1 = victoire des blancs, 2 = victoire des noirs
  RandomManager random_manager;
  ContinuousUniformDistribution move_speed_distribution;
  GeometricPromotionDistribution geometric_promotion_distribution;
  RandomPermutationDistribution random_permutation_distribution;
  ExponentialDistribution exponential_distribution;
  int moves_until_random_promotion = 1;
  std::optional<coords> last_promotion_pos;
  double last_promotion_time = 0.0;
  std::optional<coords> meteor_position;
  double meteor_start_time = -1.0;
  double meteor_next_event_time = 0.0;

  void checkKingCapture(Piece const *captured);
  void applyRandomStartPermutationIfNeeded();
  void resetGameState();
  void resetRandomPromotionCountdown();
  void applyRandomPromotionIfNeeded();
  void promoteRandomPawnToQueen();
};
