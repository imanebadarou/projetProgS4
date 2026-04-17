#include "pawn.hpp"
#include "../board.hpp"

Pawn::Pawn(const Color color) : Piece(color) {};

std::vector<std::array<int, 2>> Pawn::whereCanIMove(const Board &board,
                                                    coords position) const {
  std::vector<std::array<int, 2>> moves;
  int dir = (this->getColor() == Color::white) ? 1 : -1;

  // avance de 1
  int nx = position.x + dir;
  int ny = position.y;
  if (nx >= 0 && nx < 8) {
    if (board.getPieceFromPos({nx, ny}) == nullptr) {
      moves.push_back({nx, ny});
      // avance de 2
      if (!this->getHasMoved()) {
        int nx2 = position.x + 2 * dir;
        if (nx2 >= 0 && nx2 < 8 &&
            board.getPieceFromPos({nx2, ny}) == nullptr) {
          moves.push_back({nx2, ny});
        }
      }
    }
  }

  // capture en diagonale
  int dys[2] = {-1, 1};
  for (int i = 0; i < 2; ++i) {
    int cx = position.x + dir;
    int cy = position.y + dys[i];
    if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8) {
      const Piece *p = board.getPieceFromPos({cx, cy});
      if (p != nullptr && p->getColor() != this->getColor()) {
        moves.push_back({cx, cy});
      }
    }
  }
  return moves;
}