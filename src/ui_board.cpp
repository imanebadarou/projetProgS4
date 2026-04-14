#include "ui_board.hpp"
#include <cstdint>
#include <imgui.h>

UIBoard::UIBoard(GameLogic &game, TextureManager &textures)
    : game(game), textures(textures), selected_piece{-1, -1},
      promotion_modal_open(false), promotion_pos{-1, -1},
      promotion_color(Color::white) {}

void UIBoard::render() {
  if (!game.isGameOver()) {
    game.checkGameState();
  }

  ImGui::Begin("Chess Board", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  std::string turn_text =
      (game.getCurrentTurn() == Color::white) ? "White's Turn" : "Black's Turn";
  ImGui::Text("%s", turn_text.c_str());
  ImGui::Spacing();

  drawBoardGrid();

  ImGui::End();

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
    std::string key = color_str + "-" + game.getPieceName(p);
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

    game.getBoard().movePiece(selected_piece, position);
    selected_piece = {-1, -1};
    valid_moves.clear();

    if (is_pawn && ((piece_color == Color::white && position.x == 7) ||
                    (piece_color == Color::black && position.x == 0))) {
      promotion_modal_open = true;
      promotion_pos = position;
      promotion_color = piece_color;
    } else {
      game.setCurrentTurn(game.getCurrentTurn() == Color::white ? Color::black
                                                                : Color::white);
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
