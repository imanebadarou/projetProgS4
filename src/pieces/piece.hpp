#pragma once
#include "utils.hpp"
#include <array>
#include <vector>

class Board;

class Piece {
private:
  Color _color{};
  bool _hasMoved{false};

protected:
  // Constructeur
  Piece(const Color color);

public:
  bool getHasMoved() const;
  void setHasMoved();

  Color getColor() const;
  void setColor();

  virtual std::vector<std::array<int, 2>>
  whereCanIMove(const Board &board, coords position) const = 0;

  virtual ~Piece() = default;
};
