#include "knight.hpp"
#include "../board.hpp"

Knight::Knight (const Color color): Piece(color){};

std::vector<std::array<int, 2>> Knight::whereCanIMove(const Board& board, coords position) const {
    std::vector<std::array<int, 2>> moves;
    const int dx[] = {2, 2, -2, -2, 1, 1, -1, -1};
    const int dy[] = {1, -1, 1, -1, 2, -2, 2, -2};
    
    for (int i = 0; i < 8; ++i) {
        int nx = position.x + dx[i];
        int ny = position.y + dy[i];
        
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            const Piece* p = board.getPieceFromPos({nx, ny});
            if (p == nullptr || p->getColor() != this->getColor()) {
                moves.push_back({nx, ny});
            }
        }
    }
    return moves; 
}