#pragma once
#include "piece.hpp"
#include <array>
#include <vector>

class King : public Piece {
    public:
        King(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove(const Board& board, coords position) const override;
};