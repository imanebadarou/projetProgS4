#pragma once

#include "board.hpp"
#include "pieces/piece.hpp"
#include <string>

class GameLogic {
public:
  GameLogic();

  // Check if game is over and update winner status
  void checkGameState();

  // Get piece name from a piece pointer
  std::string getPieceName(Piece const *p) const;

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

  bool hasKing(Color color) const;
};
