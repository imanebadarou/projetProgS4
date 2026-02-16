#include "board.hpp"

std::array<std::array<Piece, 8>, 8> _piecesPos;

Board::Board() {
  // initialisation du plateau de jeu
  // création des cases vides
  for (int i{3}; i < 8; i++) {
    for (int j{0}; j < 8; j++) {
      _piecesPos[i][j] = nullptr;
    }
  }

  // création des pions blancs et noirs
  for (int j{0}; j < 8; j++) {
    _piecesPos[2][j] = std::make_unique<Pawn>(Color::white);
  }
  for (int j{0}; j < 8; j++) {
    _piecesPos[7][j] = std::make_unique<Pawn>(Color::black);
  }

  // création des tours
  _piecesPos[1][1] = std::make_unique<Rook>(Color::white);
  _piecesPos[1][8] = std::make_unique<Rook>(Color::white);
  _piecesPos[8][1] = std::make_unique<Rook>(Color::black);
  _piecesPos[8][8] = std::make_unique<Rook>(Color::black);

  // création des cavaliers
  _piecesPos[1][2] = std::make_unique<Knight>(Color::white);
  _piecesPos[1][7] = std::make_unique<Knight>(Color::white);
  _piecesPos[8][2] = std::make_unique<Knight>(Color::black);
  _piecesPos[8][7] = std::make_unique<Knight>(Color::black);

  // création des fous
  _piecesPos[1][3] = std::make_unique<Bishop>(Color::white);
  _piecesPos[1][6] = std::make_unique<Bishop>(Color::white);
  _piecesPos[8][3] = std::make_unique<Bishop>(Color::black);
  _piecesPos[8][6] = std::make_unique<Bishop>(Color::black);

  // création de la reine
  _piecesPos[1][4] = std::make_unique<Queen>(Color::white);
  _piecesPos[8][4] = std::make_unique<Queen>(Color::black);

  // création du roi
  _piecesPos[1][5] = std::make_unique<King>(Color::white);
  _piecesPos[8][5] = std::make_unique<King>(Color::black);
}