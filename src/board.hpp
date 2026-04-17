#pragma once
#include "utils.hpp"

#include <array>
#include <memory>
#include <string>

class Piece;

class Board {
private:
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> _boardPos;

public:
  Board();
  Piece const *getPieceFromPos(coords position) const;
  void movePiece(coords from, coords to);
  void removePiece(coords position);
  bool promotePawn(coords position, std::string const &pieceType, Color color);
  void setBackRankFromPermutation(std::array<int, 8> const &permutation);
};