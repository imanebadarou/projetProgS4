#include "board.hpp"

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
    _piecesPos[1][j] = std::make_unique<Pawn>(Color::white);
  }
  for (int j{0}; j < 8; j++) {
    _piecesPos[6][j] = std::make_unique<Pawn>(Color::black);
  }

  // création des tours
  _piecesPos[0][0] = std::make_unique<Rook>(Color::white);
  _piecesPos[0][7] = std::make_unique<Rook>(Color::white);
  _piecesPos[7][0] = std::make_unique<Rook>(Color::black);
  _piecesPos[7][7] = std::make_unique<Rook>(Color::black);

  // création des cavaliers
  _piecesPos[0][1] = std::make_unique<Knight>(Color::white);
  _piecesPos[0][6] = std::make_unique<Knight>(Color::white);
  _piecesPos[7][1] = std::make_unique<Knight>(Color::black);
  _piecesPos[7][6] = std::make_unique<Knight>(Color::black);

  // création des fous
  _piecesPos[0][2] = std::make_unique<Bishop>(Color::white);
  _piecesPos[0][5] = std::make_unique<Bishop>(Color::white);
  _piecesPos[7][2] = std::make_unique<Bishop>(Color::black);
  _piecesPos[7][5] = std::make_unique<Bishop>(Color::black);

  // création de la reine
  _piecesPos[0][3] = std::make_unique<Queen>(Color::white);
  _piecesPos[7][3] = std::make_unique<Queen>(Color::black);

  // création du roi
  _piecesPos[0][4] = std::make_unique<King>(Color::white);
  _piecesPos[7][4] = std::make_unique<King>(Color::black);
}

// std::vector<std::unique_ptr<Piece>> Board::getPiecesPos() {
//   std::vector<std::unique_ptr<Piece>> tabPieces;

//   for (int i{0}; i < 8; i++) {
//     for (int j{0}; j < 8; j++) {
//       if (_piecesPos[i][j] != nullptr) {
//         tabPieces.push_back(_piecesPos[i][j]);
//       }
//     }
//   }
//   return tabPieces;
// }