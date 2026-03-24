#pragma once
#include <array>
#include <vector>
#include "utils.hpp"
class Board;

enum class Color { black, white };

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

  virtual std::vector<std::array<int, 2>> whereCanIMove(const Board& board, coords position) = 0; //utiliser un std:fonction pour le board

  virtual ~Piece() = default;
};
