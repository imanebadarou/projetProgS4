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

  quick_imgui::loop("Chess 3D",
                    {.init = [&]() { 
                            textureManager.loadAllPieceTextures(); 
                            uiBoard.init3D();
                        },
                     .loop = [&]() { uiBoard.render(); },
                     .key_callback = [&](int key, int scancode, int action, int mods) {},
                     .mouse_button_callback = [&](int button, int action, int mods) { 
                         uiBoard.onMouseClick(button, action); 
                     },
                     .cursor_position_callback = [&](double xpos, double ypos) { 
                         uiBoard.onCursorMove(xpos, ypos); 
                     },
                     .scroll_callback = [&](double xoffset, double yoffset) { 
                         uiBoard.onScroll(yoffset); 
                     },
                     .window_size_callback = [&](int width, int height) { 
                         uiBoard.updateWindowSize(width, height); 
                     }
                    });

  return 0;
}