#include "ui_board.hpp"
#include <GLFW/glfw3.h> // For button constants
#include <cstdint>
#include <imgui.h>

UIBoard::UIBoard(GameLogic &game, TextureManager &textures)
    : game(game), textures(textures) {}

void UIBoard::init3D() { scene3d.init(); }

void UIBoard::syncPieceViewCamera() {
  if (!piece_view_enabled) {
    camera.setMode(CameraMode::Trackball);
    piece_view_anchor = {-1, -1};
    return;
  }

  // Si une animation est en cours et concerne notre ancre ou notre sélection, on la suit
  if (scene3d.isAnimationActive()) {
      glm::vec3 animatedPos;
      // On vérifie si la source de l'animation correspond à notre ancienne ancre
      if (scene3d.getAnimatedWorldPositionFromSource(piece_view_anchor, animatedPos)) {
          camera.setSubjectivePosition(animatedPos + glm::vec3(0.0f, 1.35f, 0.0f));
          return;
      }
  }

  if (selected_piece.x == -1) {
    camera.setMode(CameraMode::Trackball);
    piece_view_anchor = {-1, -1};
    return;
  }

  Piece const *selected = game.getBoard().getPieceFromPos(selected_piece);
  if (!selected) {
    camera.setMode(CameraMode::Trackball);
    piece_view_anchor = {-1, -1};
    return;
  }

  const bool selectionChanged = piece_view_anchor.x != selected_piece.x ||
                                 piece_view_anchor.y != selected_piece.y;

  if (camera.getMode() != CameraMode::FirstPerson || selectionChanged) {
    const float yaw = (selected->getColor() == Color::white) ? 0.0f : 180.0f;
    const float pitch = -25.0f;

    camera.setSubjectivePosition(
        glm::vec3((float)selected_piece.x, 1.75f, (float)selected_piece.y));
    camera.setSubjectiveOrientation(yaw, pitch);
    camera.setMode(CameraMode::FirstPerson);
    piece_view_anchor = selected_piece;
  }
}

void UIBoard::emitRaycastLocal(float local_x, float local_y, float width,
                               float height) {
  if (width <= 0 || height <= 0)
    return;

  // Normalised Device Coordinates
  float x = (2.0f * local_x) / width - 1.0f;
  float y = 1.0f - (2.0f * local_y) / height;

  float aspectRatio = width / height;
  glm::mat4 proj = camera.getProjectionMatrix(aspectRatio);
  glm::mat4 view = camera.getViewMatrix();

  glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
  glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
  ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

  glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
  ray_wor = glm::normalize(ray_wor);

  glm::vec3 cam_pos = camera.getPosition();

  // Intersection with y=0 plane
  if (std::abs(ray_wor.y) > 0.001f) {
    float t = -cam_pos.y / ray_wor.y;
    if (t > 0) {
      glm::vec3 p = cam_pos + t * ray_wor;
      int gridX = std::round(p.x);
      int gridZ = std::round(p.z);
      if (gridX >= 0 && gridX < 8 && gridZ >= 0 && gridZ < 8) {
        hovered_piece = {gridX, gridZ};
        return;
      }
    }
  }
  hovered_piece = {-1, -1};
}

void UIBoard::render() {

  // Draw 3D scene inside an ImGui Window
  if (show_3d) {
    ImGui::Begin("Vue 3D", &show_3d);
    ImVec2 avail = ImGui::GetContentRegionAvail();
    if (avail.x > 0 && avail.y > 0) {
      syncPieceViewCamera();

      GLuint tex =
          scene3d.renderToTexture(camera, avail.x, avail.y, game, hovered_piece,
                                  selected_piece, valid_moves);

      ImGui::Image((ImTextureID)(intptr_t)tex, avail, ImVec2(0, 1),
                   ImVec2(1, 0));

      if (ImGui::IsItemHovered()) {
        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 mousePos = ImGui::GetMousePos();

        float local_x = mousePos.x - p0.x;
        float local_y = mousePos.y - p0.y;

        emitRaycastLocal(local_x, local_y, avail.x, avail.y);

        if (ImGui::IsMouseClicked(0)) {
          if (hovered_piece.x != -1)
            handleSquareClick(hovered_piece, false);
        } else if (ImGui::IsMouseClicked(1) && !piece_view_enabled) {
          handleSquareClick({-1, -1}, true);
        }

        if (ImGui::IsMouseDragging(1)) {
          ImVec2 delta = ImGui::GetMouseDragDelta(1);
          if (camera.getMode() == CameraMode::Trackball) {
            camera.rotateTrackball(glm::radians(-delta.y * 0.5f),
                                   glm::radians(-delta.x * 0.5f));
          } else {
            // Mode pièce : rotation 360° etc.
            camera.rotateSubjective(-delta.x * 0.5f, -delta.y * 0.5f);
          }
          ImGui::ResetMouseDragDelta(1);
        }

        float scroll = ImGui::GetIO().MouseWheel;
        if (scroll != 0.0f) {
          camera.zoomTrackball(scroll * 0.5f);
        }
      } else {
        hovered_piece = {-1, -1};
      }
    }
    ImGui::End();
  }

  // Main UI Status / Settings Overlay
  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
  ImGui::Begin("Chess View Options", nullptr,
               ImGuiWindowFlags_AlwaysAutoResize);
  std::string turn_text =
      (game.getCurrentTurn() == Color::white) ? "White's Turn" : "Black's Turn";
  ImGui::Text("%s", turn_text.c_str());
  ImGui::Spacing();
  ImGui::Checkbox("Afficher vue 3D", &show_3d);
  ImGui::Checkbox("Afficher vue 2D ImGui", &show_2d);
  if (show_3d) {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Mode de Caméra :");
    int mode = piece_view_enabled ? (int)CameraMode::FirstPerson
                                  : (int)CameraMode::Trackball;
    if (ImGui::RadioButton("Trackball", mode == (int)CameraMode::Trackball)) {
      piece_view_enabled = false;
      piece_view_anchor = {-1, -1};
      camera.setMode(CameraMode::Trackball);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Vue Pièce", mode == (int)CameraMode::FirstPerson)) {
      piece_view_enabled = true;
      syncPieceViewCamera();
    }

    if (piece_view_enabled && selected_piece.x == -1) {
      ImGui::TextColored(ImVec4(1, 0.5, 0, 1), "Sélectionnez une pièce !");
    }

    ImGui::Spacing();
    ImGui::Text("--- Contrôles 3D ---");
    ImGui::Text("Clic Droit + Drag : Rotation");
    if (camera.getMode() == CameraMode::Trackball) {
      ImGui::Text("Molette : Zoom");
    }
  }
  ImGui::End();

  // Show the 2D UI Board if enabled
  if (show_2d) {
    ImGui::Begin("Chess Board 2D", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    drawBoardGrid();
    ImGui::End();
  }

  if (game.isGameOver()) {
    drawGameOverWindow();
  }

  drawPromotionModal();
}

void UIBoard::drawBoardGrid() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

  for (int x = 7; x >= 0; --x) {
    for (int y = 0; y < 8; ++y) {
      bool isSelected = (selected_piece.x == x && selected_piece.y == y);
      bool isValidMove = false;

      for (const auto &move : valid_moves) {
        if (move[0] == x && move[1] == y) {
          isValidMove = true;
          break;
        }
      }

      renderSquare(x, y, isSelected, isValidMove);

      if (y < 7) {
        ImGui::SameLine();
      }
    }
  }
  ImGui::PopStyleVar(2);
}

void UIBoard::renderSquare(int x, int y, bool isSelected, bool isValidMove) {
  ImGui::PushID(x * 8 + y);

  bool isBlackSquare = ((x + y) % 2 == 0);
  ImVec4 square_color;

  if (isSelected) {
    square_color = ImVec4(0.85f, 0.85f, 0.2f, 1.0f);
  } else if (isValidMove) {
    square_color = isBlackSquare ? ImVec4(0.3f, 0.5f, 0.7f, 1.0f)
                                 : ImVec4(0.5f, 0.7f, 0.9f, 1.0f);
  } else {
    square_color = isBlackSquare ? ImVec4(0.46f, 0.58f, 0.33f, 1.0f)
                                 : ImVec4(0.93f, 0.93f, 0.82f, 1.0f);
  }

  ImGui::PushStyleColor(ImGuiCol_Button, square_color);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.4f, 1.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.6f, 0.2f, 1.f));

  Piece const *p = game.getBoard().getPieceFromPos({x, y});

  if (p) {
    std::string color_str = (p->getColor() == Color::white) ? "white" : "black";
    std::string key = color_str + "-" + GameLogic::getPieceName(p);
    GLuint tex = textures.getTexture(key);

    if (ImGui::ImageButton("##piece", (ImTextureID)(intptr_t)tex,
                           ImVec2(64, 64), ImVec2(0, 0), ImVec2(1, 1),
                           square_color)) {
      handleSquareClick({x, y}, false);
    }
  } else {
    if (ImGui::Button("##empty", ImVec2(64, 64))) {
      handleSquareClick({x, y}, false);
    }
  }

  if (ImGui::IsItemClicked(1)) {
    handleSquareClick({x, y}, true);
  }

  ImGui::PopStyleColor(3);
  ImGui::PopID();
}

void UIBoard::handleSquareClick(coords position, bool isRightClick) {
  if (game.isGameOver() || promotion_modal_open)
    return;

  if (isRightClick) {
    selected_piece = {-1, -1};
    valid_moves.clear();
    syncPieceViewCamera();
    return;
  }

  bool isValidMove = false;
  for (const auto &move : valid_moves) {
    if (move[0] == position.x && move[1] == position.y) {
      isValidMove = true;
      break;
    }
  }

  if (isValidMove) {
    Piece const *moving_piece = game.getBoard().getPieceFromPos(selected_piece);
    bool is_pawn = false;
    Color piece_color = Color::white;
    if (moving_piece) {
      is_pawn = (game.getPieceName(moving_piece) == "pawn");
      piece_color = moving_piece->getColor();
    }

    scene3d.pushAnimation(selected_piece, position);

    game.makeMove(selected_piece, position);
    
    // Si on est en "Vue Pièce", on veut rester attaché à la pièce qu'on vient de bouger.
    // On met à jour selected_piece vers la destination au lieu de la reset à -1.
    if (piece_view_enabled) {
        selected_piece = position;
    } else {
        selected_piece = {-1, -1};
    }
    
    valid_moves.clear();

    if (is_pawn && ((piece_color == Color::white && position.x == 7) ||
                    (piece_color == Color::black && position.x == 0))) {
      promotion_modal_open = true;
      promotion_pos = position;
      promotion_color = piece_color;
    }
  } else {
    Piece const *p = game.getBoard().getPieceFromPos(position);
    if (p && p->getColor() == game.getCurrentTurn()) {
      selected_piece = position;
      valid_moves = p->whereCanIMove(game.getBoard(), position);
    } else {
      selected_piece = {-1, -1};
      valid_moves.clear();
    }
  }

  syncPieceViewCamera();
}

void UIBoard::drawGameOverWindow() {
  ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  if (game.getWinner() == 1) {
    ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.2f, 1.0f),
                       "Congratulations, White Wins!");
  } else {
    ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f),
                       "Congratulations, Black Wins!");
  }

  ImGui::Spacing();
  if (ImGui::Button("Play Again", ImVec2(120, 30))) {
    game.resetGame();
    selected_piece = {-1, -1};
    valid_moves.clear();
    promotion_modal_open = false;
  }
  ImGui::End();
}

void UIBoard::drawPromotionModal() {
  if (promotion_modal_open && !ImGui::IsPopupOpen("Pawn Promotion")) {
    ImGui::OpenPopup("Pawn Promotion");
  }

  if (ImGui::BeginPopupModal("Pawn Promotion", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::Text("Choose a piece for promotion:");
    ImGui::Spacing();

    std::string color_str =
        (promotion_color == Color::white) ? "white" : "black";
    std::vector<std::string> pieces = {"queen", "rook", "bishop", "knight"};

    std::string chosen_piece = "";

    for (size_t i = 0; i < pieces.size(); ++i) {
      std::string key = color_str + "-" + pieces[i];
      GLuint tex = textures.getTexture(key);

      ImGui::PushID(static_cast<int>(i));
      if (ImGui::ImageButton("##promotion", (ImTextureID)(intptr_t)tex,
                             ImVec2(64, 64))) {
        chosen_piece = pieces[i];
      }
      ImGui::PopID();

      if (i < pieces.size() - 1) {
        ImGui::SameLine();
      }
    }

    if (!chosen_piece.empty()) {
      game.getBoard().promotePawn(promotion_pos, chosen_piece, promotion_color);
      promotion_modal_open = false;
      game.setCurrentTurn(game.getCurrentTurn() == Color::white ? Color::black
                                                                : Color::white);
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}