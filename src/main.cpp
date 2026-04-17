#include "game_logic.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "random/policies/starting_player_policy.hpp"
#include "random/random_manager.hpp"
#include "rendering/texture_manager.hpp"
#include "ui/mode_selector.hpp"
#include "ui/ui_board.hpp"

int main() {
  GameLogic game;
  TextureManager textureManager;
  UIBoard uiBoard(game, textureManager);
  ModeSelector modeSelector;
  RandomManager randomManager;
  StartingPlayerPolicy startingPlayerPolicy;

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
                 const GameMode selected_mode = modeSelector.getSelectedMode();
                 game.setGameMode(selected_mode);

                 if (selected_mode == GameMode::RANDOM) {
                   const double random_01 = randomManager.generateUniform();
                   const double p =
                       startingPlayerPolicy.computeLowerEloStartProbability(
                           static_cast<double>(modeSelector.getWhiteElo()),
                           static_cast<double>(modeSelector.getBlackElo()));

                   const bool lower_elo_starts =
                       randomManager.bernoulliFromRandom(random_01, p);

                   const bool white_starts =
                       startingPlayerPolicy.shouldWhiteStart(
                           static_cast<double>(modeSelector.getWhiteElo()),
                           static_cast<double>(modeSelector.getBlackElo()),
                           lower_elo_starts);

                   game.setCurrentTurn(white_starts ? Color::white
                                                    : Color::black);
                 } else {
                   game.setCurrentTurn(Color::white);
                 }

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