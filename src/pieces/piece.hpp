#include <array>
#include <vector>
enum class Color { black, white };

class Piece {
public:
  Color _color{Color::black};
  bool _hasMoved{false};

public:
  void setHasMoved();
  std::vector<std::array<int, 2>> whereCanIMove();
};
