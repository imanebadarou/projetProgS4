#include "piece.hpp"
#include <array>
#include <vector>

class Rook : public Piece {
    public:
        Rook(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove() override;
};