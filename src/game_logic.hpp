#pragma once

#include "board.hpp"
#include "game_mode.hpp"
#include "pieces/piece.hpp"
#include "probability_distribution/continuous_uniform_distribution.hpp"
#include "probability_distribution/geometric_promotion_distribution.hpp"
#include "probability_distribution/random_permutation_distribution.hpp"
#include "random.hpp"
#include <string>

class GameLogic {
public:
  GameLogic();

  // Execute a move and update game state
  bool makeMove(coords from, coords to);

  // Get piece name from a piece pointer
  static std::string getPieceName(Piece const *piece);

  // Getters
  bool isGameOver() const { return winner != 0; }
  int getWinner() const { return winner; }
  Color getCurrentTurn() const { return current_turn; }
  Board &getBoard() { return board; }
  GameMode getGameMode() const { return game_mode; }
  coords getLastRandomPromotionPos() const { return last_promotion_pos; }
  double getLastPromotionTime() const { return last_promotion_time; }
  bool hasRandomPromotionOccurred() const { return last_promotion_pos.x != -1; }
  double sampleMoveSpeedFactor() const;

  // Setters
  void setCurrentTurn(Color color) { current_turn = color; }
  void setGameMode(GameMode mode);
  void resetGame();

private:
  Board board;
  Color current_turn;
  int winner; // 0 = playing, 1 = white wins, 2 = black wins
  GameMode game_mode = GameMode::NORMAL;
  RandomManager random_manager;
  ContinuousUniformDistribution move_speed_distribution;
  GeometricPromotionDistribution geometric_promotion_distribution;
  RandomPermutationDistribution random_permutation_distribution;
  int moves_until_random_promotion = 1;
  coords last_promotion_pos{-1, -1};
  double last_promotion_time = 0.0;

  void checkKingCapture(Piece const *captured);
  void applyRandomStartPermutationIfNeeded();
  void resetRandomPromotionCountdown();
  void applyRandomPromotionIfNeeded();
  void promoteRandomPawnToQueen();
};
