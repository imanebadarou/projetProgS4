#include "piece.hpp"

Piece::Piece (const Color color): _color(color){};

void Piece::setHasMoved() { _hasMoved = true; }

bool Piece::getHasMoved() const { return _hasMoved; }

Color Piece::getColor() const { return _color; }
