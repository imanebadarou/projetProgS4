#include "piece.hpp"
#include <array>
#include <vector>

class Bishop : public Piece {
    public:
        Bishop(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove() override;
};