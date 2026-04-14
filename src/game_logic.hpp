#pragma once

#include "board.hpp"
#include "pieces/piece.hpp"
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

  // Setters
  void setCurrentTurn(Color color) { current_turn = color; }
  void resetGame();

private:
  Board board;
  Color current_turn;
  int winner; // 0 = playing, 1 = white wins, 2 = black wins

  void checkKingCapture(Piece const *captured);
};
