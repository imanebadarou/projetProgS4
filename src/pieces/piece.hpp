#include <array>
#include <vector>

enum class Color { black, white };

class Piece {
private:
  Color _color{};
  bool _hasMoved{false};

protected:
  Piece(const Color color);

public:
  void setHasMoved();
  std::vector<std::array<int, 2>> whereCanIMove();
};


