#pragma once
#include "piece.hpp"
#include <array>
#include <vector>

class Knight : public Piece {
    public:
        Knight(const Color color);
        std::vector<std::array<int, 2>> whereCanIMove(const Board& board, coords position) override;
};