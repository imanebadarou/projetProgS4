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

  bool promotion_modal_open;
  coords promotion_pos;
  Color promotion_color;

  void drawBoardGrid();
  void drawGameOverWindow();
  void drawPromotionModal();
  void renderSquare(int x, int y, bool isSelected, bool isValidMove);
};
