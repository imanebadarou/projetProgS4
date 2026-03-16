#include "board.hpp"

Board::Board() {
  // initialisation du plateau de jeu
  // création des cases vides
  // for (int i{2}; i < 6; i++) {
  //   for (int j{0}; j < 8; j++) {
  //     _boardPos[i][j] = nullptr;
  //   }
  // }

  // création des pions blancs et noirs
  for (int j{0}; j < 8; j++) {
    _boardPos[1][j] = std::make_unique<Pawn>(Color::white);
  }
  for (int j{0}; j < 8; j++) {
    _boardPos[6][j] = std::make_unique<Pawn>(Color::black);
  }

  // création des tours
  _boardPos[0][0] = std::make_unique<Rook>(Color::white);
  _boardPos[0][7] = std::make_unique<Rook>(Color::white);
  _boardPos[7][0] = std::make_unique<Rook>(Color::black);
  _boardPos[7][7] = std::make_unique<Rook>(Color::black);

  // création des cavaliers
  _boardPos[0][1] = std::make_unique<Knight>(Color::white);
  _boardPos[0][6] = std::make_unique<Knight>(Color::white);
  _boardPos[7][1] = std::make_unique<Knight>(Color::black);
  _boardPos[7][6] = std::make_unique<Knight>(Color::black);

  // création des fous
  _boardPos[0][2] = std::make_unique<Bishop>(Color::white);
  _boardPos[0][5] = std::make_unique<Bishop>(Color::white);
  _boardPos[7][2] = std::make_unique<Bishop>(Color::black);
  _boardPos[7][5] = std::make_unique<Bishop>(Color::black);

  // création de la reine
  _boardPos[0][3] = std::make_unique<Queen>(Color::white);
  _boardPos[7][3] = std::make_unique<Queen>(Color::black);

  // création du roi
  _boardPos[0][4] = std::make_unique<King>(Color::white);
  _boardPos[7][4] = std::make_unique<King>(Color::black);
}

Piece const *Board::getPieceFromPos(coords coords) {
  return _boardPos[coords.x][coords.y].get();
}
