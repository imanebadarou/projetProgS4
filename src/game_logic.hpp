#pragma once

#include "board.hpp"
#include "game_mode.hpp"
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
  GameMode getGameMode() const { return game_mode; }

  // Setters
  void setCurrentTurn(Color color) { current_turn = color; }
  void setGameMode(GameMode mode) { game_mode = mode; }
  void resetGame();

private:
  Board board;
  Color current_turn;
  int winner; // 0 = playing, 1 = white wins, 2 = black wins
  GameMode game_mode = GameMode::NORMAL;

  void checkKingCapture(Piece const *captured);
};
