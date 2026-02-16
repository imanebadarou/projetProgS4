#include "pieces\piece.hpp"

class Board {
private:
  std::array<std::array<Piece, 8>, 8> _piecesPos;

public:
  Board();
};