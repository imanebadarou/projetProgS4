#include "game_logic.hpp"
#include "game_mode_manager.hpp"
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"


GameLogic::GameLogic() : current_turn(Color::white), winner(0) {
  game_mode = GameModeManager::getInstance().getGameMode();
}

bool GameLogic::makeMove(coords from, coords to) {
  // Check if there's a piece at destination (capture)
  Piece const *captured = board.getPieceFromPos(to);
  checkKingCapture(captured);

  // Execute the move
  board.movePiece(from, to);

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
}
