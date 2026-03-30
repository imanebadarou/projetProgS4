#include "pieces/piece.hpp"
#include "pieces/pawn.hpp"
#include "pieces/rook.hpp"
#include "pieces/knight.hpp"
#include "pieces/bishop.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include <imgui.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "board.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

GLuint loadTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    return texture;
}

std::string get_piece_name(Piece const* p) {
    if (dynamic_cast<Pawn const*>(p)) return "pawn";
    if (dynamic_cast<Rook const*>(p)) return "rook";
    if (dynamic_cast<Knight const*>(p)) return "knight";
    if (dynamic_cast<Bishop const*>(p)) return "bishop";
    if (dynamic_cast<Queen const*>(p)) return "queen";
    if (dynamic_cast<King const*>(p)) return "king";
    return "";
}

int main() {
    Board board;
    std::map<std::string, GLuint> textures;

    std::vector<std::array<int, 2>> valid_moves;
    coords selected_piece{-1, -1};
    Color current_turn = Color::white;
    int winner = 0; // 0 = playing, 1 = white wins, 2 = black wins

    quick_imgui::loop("Chess", {
        .init = [&]() {
            // Load all textures
            std::vector<std::string> colors = {"white", "black"};
            std::vector<std::string> names = {"pawn", "rook", "knight", "bishop", "queen", "king"};
            for (const auto& color : colors) {
                for (const auto& name : names) {
                    std::string key = color + "-" + name;
                    // Adjusted path to look two levels up since executable is in bin/Debug/
                    std::string path = "../../assets/pieces/" + key + ".png";
                    textures[key] = loadTexture(path.c_str());
                }
            }
        },
        .loop = [&]() {
            if (winner == 0) {
                bool white_k = false, black_k = false;
                for (int i=0; i<8; ++i) {
                    for (int j=0; j<8; ++j) {
                        Piece const* pcheck = board.getPieceFromPos({i, j});
                        if (pcheck && get_piece_name(pcheck) == "king") {
                            if (pcheck->getColor() == Color::white) white_k = true;
                            else black_k = true;
                        }
                    }
                }
                if (!white_k) winner = 2; // Black wins
                else if (!black_k) winner = 1; // White wins
            }

            ImGui::Begin("Chess Board", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            
            std::string turn_text = (current_turn == Color::white) ? "White's Turn" : "Black's Turn";
            ImGui::Text("%s", turn_text.c_str());
            ImGui::Spacing();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // No margin
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // No padding inside button
            
            // Draw from top (row 7 for black back rank) to bottom (row 0 for white back rank)
            for (int x = 7; x >= 0; --x) {
                for (int y = 0; y < 8; ++y) {
                    ImGui::PushID(x * 8 + y);
                    
                    bool isSelected = (selected_piece.x == x && selected_piece.y == y);
                    bool isValidMove = false;
                    for (const auto& move : valid_moves) {
                        if (move[0] == x && move[1] == y) {
                            isValidMove = true;
                            break;
                        }
                    }

                    bool isBlackSquare = ((x + y) % 2 == 0);
                    ImVec4 square_color;
                    
                    if (isSelected) {
                        square_color = ImVec4(0.85f, 0.85f, 0.2f, 1.0f); // Bright yellow for selected
                    } else if (isValidMove) {
                        square_color = isBlackSquare ? ImVec4(0.3f, 0.5f, 0.7f, 1.0f) // Blue on dark
                                                     : ImVec4(0.5f, 0.7f, 0.9f, 1.0f); // Blue on light
                    } else {
                        square_color = isBlackSquare ? ImVec4(0.46f, 0.58f, 0.33f, 1.0f) // Dark green-ish
                                                     : ImVec4(0.93f, 0.93f, 0.82f, 1.0f); // Light cream
                    }
                    
                    ImGui::PushStyleColor(ImGuiCol_Button, square_color);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.4f, 1.f)); // Highlight
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.6f, 0.2f, 1.f));
                    
                    Piece const* p = board.getPieceFromPos({x, y});
                    
                    if (p) {
                        std::string color_str = (p->getColor() == Color::white) ? "white" : "black";
                        std::string key = color_str + "-" + get_piece_name(p);
                        GLuint tex = textures[key];
                        
                        if (ImGui::ImageButton("##piece", (ImTextureID)(intptr_t)tex, ImVec2(64, 64), 
                                           ImVec2(0,0), ImVec2(1,1), square_color)) {
                            if (winner == 0) {
                                if (isValidMove) {
                                    board.movePiece(selected_piece, {x, y});
                                    selected_piece = {-1, -1};
                                    valid_moves.clear();
                                    current_turn = (current_turn == Color::white) ? Color::black : Color::white;
                                } else if (p->getColor() == current_turn) {
                                    selected_piece = {x, y};
                                    valid_moves = p->whereCanIMove(board, {x, y});
                                } else {
                                    selected_piece = {-1, -1};
                                    valid_moves.clear();
                                }
                            }
                        }
                    } else {
                        if (ImGui::Button("##empty", ImVec2(64, 64))) {
                            if (winner == 0) {
                                if (isValidMove) {
                                    board.movePiece(selected_piece, {x, y});
                                    selected_piece = {-1, -1};
                                    valid_moves.clear();
                                    current_turn = (current_turn == Color::white) ? Color::black : Color::white;
                                } else {
                                    selected_piece = {-1, -1};
                                    valid_moves.clear();
                                }
                            }
                        }
                    }

                    if (ImGui::IsItemClicked(1)) { // Right click to deselect
                        selected_piece = {-1, -1};
                        valid_moves.clear();
                    }
                    
                    ImGui::PopStyleColor(3);
                    
                    if (y < 7) {
                        ImGui::SameLine();
                    }
                    ImGui::PopID();
                }
            }
            ImGui::PopStyleVar(2);

            ImGui::End();

            if (winner != 0) {
                ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                if (winner == 1) {
                    ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.2f, 1.0f), "Congratulations, White Wins!");
                } else {
                    ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "Congratulations, Black Wins!");
                }
                
                ImGui::Spacing();
                if (ImGui::Button("Play Again", ImVec2(120, 30))) {
                    board = Board(); // Reset board
                    valid_moves.clear();
                    selected_piece = {-1, -1};
                    current_turn = Color::white;
                    winner = 0;
                }
                ImGui::End();
            }
        }
    });

    return 0;
}