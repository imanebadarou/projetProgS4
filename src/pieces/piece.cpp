#include "piece.hpp"

Piece::Piece (const Color color): _color(color){};

void Piece::setHasMoved() { _hasMoved = true; };