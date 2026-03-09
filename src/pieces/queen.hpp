#include "piece.hpp"
#include <array>
#include <vector>

class Queen : public Piece {
    public:
        Queen(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove() override;
};