#pragma once
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
// #include "pieces/piece.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"
#include "utils.hpp"

#include <memory>

class Piece;

class Board {
private:
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> _boardPos;

public:
  Board();
  Piece const *getPieceFromPos(coords position) const;
};