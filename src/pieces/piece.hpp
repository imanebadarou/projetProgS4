#pragma once
#include <array>
#include <vector>
enum class Color { black, white };

class Piece {
private:
  Color _color{};
  bool _hasMoved{false};

protected:
  // Constructeur 
  Piece(const Color color);

public:
  bool getHasMoved();
  void setHasMoved();

  Color getColor();
  void setColor();

  virtual std::vector<std::array<int, 2>> whereCanIMove() = 0;

  virtual ~Piece() = default;
};
