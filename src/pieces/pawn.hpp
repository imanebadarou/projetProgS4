#include "piece.hpp"
#include <array>
#include <vector>

class Pawn : public Piece {
    public:
        Pawn(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove() override;
};