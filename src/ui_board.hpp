#pragma once

#include "game_logic.hpp"
#include "texture_manager.hpp"
#include "utils.hpp"
#include <array>
#include <vector>


class UIBoard {
public:
  UIBoard(GameLogic &game, TextureManager &textures);

  void render();
  void handleSquareClick(coords position, bool isRightClick);

private:
  GameLogic &game;
  TextureManager &textures;

  std::vector<std::array<int, 2>> valid_moves;
  coords selected_piece;

  void drawBoardGrid();
  void drawGameOverWindow();
  void renderSquare(int x, int y, bool isSelected, bool isValidMove);
};
