#pragma once

#include "camera.hpp"
#include "game_logic.hpp"
#include "scene_3d.hpp"
#include "texture_manager.hpp"
#include <array>
#include <optional>
#include <vector>

class UIBoard {
public:
  UIBoard(GameLogic &game, TextureManager &textures);

  void init3D();
  void render(); // Renders Both 3D & UI
  void handleSquareClick(coords position, bool isRightClick);

  void updateWindowSize(int w, int h) {}
  void updateCursorPos(double x, double y) {}
  void onMouseClick(int button, int action) {}
  void onScroll(double yoffset) {}
  void onCursorMove(double x, double y) {}

private:
  GameLogic &game;
  TextureManager &textures;

  Camera camera;
  Scene3D scene3d;

  std::vector<std::array<int, 2>> valid_moves;
  std::optional<coords> selected_piece;
  std::optional<coords> hovered_piece;

  bool promotion_modal_open{false};
  std::optional<coords> promotion_pos;
  Color promotion_color{Color::white};

  int window_width{800}, window_height{800};

  void emitRaycastLocal(float local_x, float local_y, float width,
                        float height);
  void drawGameOverWindow();
  void drawPromotionModal();

  bool show_2d{true};
  bool show_3d{true};
  bool piece_view_enabled{false};
  std::optional<coords> piece_view_anchor;

  void syncPieceViewCamera();

  double last_notification_time = -10.0;

  // L'ancienne grille 2D
  void drawBoardGrid();
  void renderSquare(int x, int y, bool isSelected, bool isValidMove);
  void drawSurprisePromotionNotification();
};
