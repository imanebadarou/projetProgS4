#include <array>
#include <vector>
enum class Color { black, white };

class Piece {
private:
  const Color _color{};
  bool _hasMoved{false};

public:
  void setHasMoved();
  std::vector<std::array<int, 2>> getPosition();
  std::vector<std::array<int, 2>> getPossibleMoves();
};
