#include "ui/mode_selector.hpp"

#include <algorithm>
#include <imgui.h>

ModeSelector::ModeSelector() {}

void ModeSelector::resetSelection() { mode_selected = false; }

bool ModeSelector::render() {
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                 ImGui::GetIO().DisplaySize.y * 0.5f),
                          ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);

  if (ImGui::Begin("Sélection du Mode", nullptr,
                   ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Bienvenue au Jeu d'Échecs 3D !");
    ImGui::Spacing();
    ImGui::Text("Joueur 1 = Blanc | Joueur 2 = Noir");
    ImGui::InputInt("ELO Blanc", &white_elo);
    ImGui::InputInt("ELO Noir", &black_elo);
    white_elo = std::max(1, white_elo);
    black_elo = std::max(1, black_elo);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Sélectionnez le mode de jeu :");
    ImGui::Spacing();

    // Mode Normal
    if (ImGui::Button("Mode Normal", ImVec2(200, 50))) {
      selected_mode = GameMode::NORMAL;
      mode_selected = true;
    }
    ImGui::TextWrapped(
        "Jouez au jeu d'échecs classique avec les règles standards.");
    ImGui::Spacing();
    ImGui::Spacing();

    // Mode Aléatoire
    if (ImGui::Button("Mode Aléatoire", ImVec2(200, 50))) {
      selected_mode = GameMode::RANDOM;
      mode_selected = true;
    }
    ImGui::TextWrapped(
        "Jouez avec des composantes aléatoires influençant la partie.");

    ImGui::End();
  }

  return mode_selected;
}
