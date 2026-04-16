#include "game_logic.hpp"
#include "game_mode_manager.hpp"
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"

#include <chrono>
#include <iostream>
#include <vector>

GameLogic::GameLogic() : current_turn(Color::white), winner(0) {
  game_mode = GameModeManager::getInstance().getGameMode();
  applyRandomStartPermutationIfNeeded();
  resetRandomPromotionCountdown();
}

double GameLogic::sampleMoveSpeedFactor() const {
  if (game_mode != GameMode::RANDOM) {
    return 1.0;
  }

  return move_speed_distribution.sample(random_manager);
}

bool GameLogic::makeMove(coords from, coords to) {
  // Check if there's a piece at destination (capture)
  Piece const *captured = board.getPieceFromPos(to);
  checkKingCapture(captured);

  // Execute the move
  board.movePiece(from, to);

  applyRandomPromotionIfNeeded();

  // Switch turn
  current_turn = (current_turn == Color::white) ? Color::black : Color::white;

  return true; // Move successful
}

std::string GameLogic::getPieceName(Piece const *piece) {
  if (!piece)
    return "";

  if (dynamic_cast<Pawn const *>(piece))
    return "pawn";
  if (dynamic_cast<Rook const *>(piece))
    return "rook";
  if (dynamic_cast<Knight const *>(piece))
    return "knight";
  if (dynamic_cast<Bishop const *>(piece))
    return "bishop";
  if (dynamic_cast<Queen const *>(piece))
    return "queen";
  if (dynamic_cast<King const *>(piece))
    return "king";

  return "";
}

void GameLogic::checkKingCapture(Piece const *captured) {
  if (captured && dynamic_cast<King const *>(captured)) {
    // King captured - game over
    winner = (captured->getColor() == Color::white) ? 2 : 1;
  }
}

void GameLogic::resetGame() {
  board = Board();
  current_turn = Color::white;
  winner = 0;
  game_mode = GameModeManager::getInstance().getGameMode();
  applyRandomStartPermutationIfNeeded();
  last_promotion_pos = {-1, -1};
  last_promotion_time = 0.0;
  resetRandomPromotionCountdown();
}

void GameLogic::setGameMode(GameMode mode) {
  game_mode = mode;
  board = Board();
  applyRandomStartPermutationIfNeeded();
  last_promotion_pos = {-1, -1};
  last_promotion_time = 0.0;
  resetRandomPromotionCountdown();
}

void GameLogic::applyRandomStartPermutationIfNeeded() {
  if (game_mode != GameMode::RANDOM) {
    return;
  }

  const std::array<int, 8> permutation =
      random_permutation_distribution.sampleBackRankPermutation(random_manager);
  board.setBackRankFromPermutation(permutation);
}

void GameLogic::resetRandomPromotionCountdown() {
  const double u = random_manager.generateUniform();
  moves_until_random_promotion =
      geometric_promotion_distribution.sampleMovesUntilPromotion(u);
}

void GameLogic::applyRandomPromotionIfNeeded() {
  if (game_mode != GameMode::RANDOM || isGameOver()) {
    return;
  }

  std::cout << moves_until_random_promotion << std::endl;
  --moves_until_random_promotion;
  if (moves_until_random_promotion > 0) {
    return;
  }

  promoteRandomPawnToQueen();
  resetRandomPromotionCountdown();
}

void GameLogic::promoteRandomPawnToQueen() {
  std::vector<coords> pawn_positions;
  pawn_positions.reserve(16);

  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      coords position{x, y};
      Piece const *piece = board.getPieceFromPos(position);
      if (piece && dynamic_cast<Pawn const *>(piece)) {
        pawn_positions.push_back(position);
      }
    }
  }

  if (pawn_positions.empty()) {
    return;
  }

  size_t random_index = static_cast<size_t>(random_manager.generateUniform() *
                                            pawn_positions.size());
  if (random_index >= pawn_positions.size()) {
    random_index = pawn_positions.size() - 1;
  }

  const coords selected = pawn_positions[random_index];
  Piece const *pawn = board.getPieceFromPos(selected);
  if (!pawn) {
    return;
  }

  board.promotePawn(selected, "queen", pawn->getColor());

  // Track the promotion for UI notification
  last_promotion_pos = selected;
  last_promotion_time =
      std::chrono::duration<double>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
}
