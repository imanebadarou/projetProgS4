#include "game_logic.hpp"
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"

GameLogic::GameLogic() : current_turn(Color::white), winner(0) {}

void GameLogic::checkGameState() {
  if (winner == 0) {
    bool white_k = hasKing(Color::white);
    bool black_k = hasKing(Color::black);

    if (!white_k) {
      winner = 2; // Black wins
    } else if (!black_k) {
      winner = 1; // White wins
    }
  }
}

std::string GameLogic::getPieceName(Piece const *piece) const {
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

bool GameLogic::hasKing(Color color) const {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Piece const *piece = board.getPieceFromPos({i, j});
      if (piece && dynamic_cast<King const *>(piece) &&
          piece->getColor() == color) {
        return true;
      }
    }
  }
  return false;
}

void GameLogic::resetGame() {
  board = Board();
  current_turn = Color::white;
  winner = 0;
}
