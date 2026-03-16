#pragma once
#include "pieces/bishop.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/piece.hpp"
#include "pieces/queen.hpp"
#include "pieces/rook.hpp"

#include <memory>

class Board {
private:
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> _piecesPos;

public:
  Board();

  std::vector<std::array<int, 2>> getPiecesPos();
};