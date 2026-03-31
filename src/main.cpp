#include "game_logic.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "texture_manager.hpp"
#include "ui_board.hpp"

int main() {
  GameLogic game;
  TextureManager textureManager;
  UIBoard uiBoard(game, textureManager);

  quick_imgui::loop("Chess",
                    {.init = [&]() { textureManager.loadAllPieceTextures(); },
                     .loop = [&]() { uiBoard.render(); }});

  return 0;
}