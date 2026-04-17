#include "board.hpp"

#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"

namespace {

constexpr std::array<int, 8> kClassicBackRank = {0, 1, 2, 3, 4, 2, 1, 0};

using BoardGrid = std::array<std::array<std::unique_ptr<Piece>, 8>, 8>;

void initializePawns(BoardGrid &board_positions) {
  for (int column = 0; column < 8; ++column) {
    board_positions[1][column] = std::make_unique<Pawn>(Color::white);
    board_positions[6][column] = std::make_unique<Pawn>(Color::black);
  }
}

enum class BackRankPiece {
  Rook = 0,
  Knight = 1,
  Bishop = 2,
  Queen = 3,
  King = 4
};

std::unique_ptr<Piece> createBackRankPiece(BackRankPiece piece_type,
                                           Color color) {
  switch (piece_type) {
  case BackRankPiece::Rook:
    return std::make_unique<Rook>(color);
  case BackRankPiece::Knight:
    return std::make_unique<Knight>(color);
  case BackRankPiece::Bishop:
    return std::make_unique<Bishop>(color);
  case BackRankPiece::Queen:
    return std::make_unique<Queen>(color);
  case BackRankPiece::King:
    return std::make_unique<King>(color);
  default:
    return std::make_unique<Rook>(color);
  }
}

} // namespace

Board::Board() {
  initializePawns(_boardPos);
  setBackRankFromPermutation(kClassicBackRank);
}

Piece const *Board::getPieceFromPos(coords position) const {
  return _boardPos[position.x][position.y].get();
}

void Board::movePiece(coords from, coords to) {
  _boardPos[to.x][to.y] = std::move(_boardPos[from.x][from.y]);
  _boardPos[to.x][to.y]->setHasMoved();
  _boardPos[from.x][from.y] = nullptr;
}

void Board::removePiece(coords position) {
  _boardPos[position.x][position.y] = nullptr;
}

bool Board::promotePawn(coords position, std::string const &pieceType,
                        Color color) {
  if (pieceType == "queen") {
    _boardPos[position.x][position.y] = std::make_unique<Queen>(color);
    return true;
  } else if (pieceType == "rook") {
    _boardPos[position.x][position.y] = std::make_unique<Rook>(color);
    return true;
  } else if (pieceType == "bishop") {
    _boardPos[position.x][position.y] = std::make_unique<Bishop>(color);
    return true;
  } else if (pieceType == "knight") {
    _boardPos[position.x][position.y] = std::make_unique<Knight>(color);
    return true;
  }

  return false;
}

void Board::setBackRankFromPermutation(std::array<int, 8> const &permutation) {
  for (int column = 0; column < 8; ++column) {
    BackRankPiece piece_type = static_cast<BackRankPiece>(permutation[column]);

    _boardPos[0][column] = createBackRankPiece(piece_type, Color::white);
    _boardPos[7][column] = createBackRankPiece(piece_type, Color::black);
  }
}