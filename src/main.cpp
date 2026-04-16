#include "game_logic.hpp"
#include "game_mode_manager.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "mode_selector.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "texture_manager.hpp"
#include "ui_board.hpp"

int main() {
  GameLogic game;
  TextureManager textureManager;
  UIBoard uiBoard(game, textureManager);
  ModeSelector modeSelector;

  bool game_started = false;
  bool show_mode_selector = true;

  quick_imgui::loop(
      "Chess 3D",
      {.init =
           [&]() {
             textureManager.loadAllPieceTextures();
             uiBoard.init3D();
           },
       .loop =
           [&]() {
             if (show_mode_selector) {
               if (modeSelector.render()) {
                 GameModeManager::getInstance().setGameMode(
                     modeSelector.getSelectedMode());
                 game_started = true;
                 show_mode_selector = false;
               }
             } else if (game_started) {
               uiBoard.render();

               // Bouton pour retourner à la sélection du mode
               ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
               if (ImGui::Begin("Menu", nullptr,
                                ImGuiWindowFlags_AlwaysAutoResize)) {
                 if (ImGui::Button("Retour au menu")) {
                   game.resetGame();
                   modeSelector.resetSelection();
                   show_mode_selector = true;
                   game_started = false;
                 }
                 ImGui::End();
               }
             }
           },
       .key_callback = [&](int key, int scancode, int action, int mods) {},
       .mouse_button_callback =
           [&](int button, int action, int mods) {
             if (game_started) {
               uiBoard.onMouseClick(button, action);
             }
           },
       .cursor_position_callback =
           [&](double xpos, double ypos) {
             if (game_started) {
               uiBoard.onCursorMove(xpos, ypos);
             }
           },
       .scroll_callback =
           [&](double xoffset, double yoffset) {
             if (game_started) {
               uiBoard.onScroll(yoffset);
             }
           },
       .window_size_callback =
           [&](int width, int height) {
             if (game_started) {
               uiBoard.updateWindowSize(width, height);
             }
           }});

  return 0;
}