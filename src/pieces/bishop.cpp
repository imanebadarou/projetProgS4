#include "bishop.hpp"
#include "../board.hpp"

Bishop::Bishop (const Color color): Piece(color){};

std::vector<std::array<int, 2>> Bishop::whereCanIMove(const Board& board, coords position) {
    std::vector<std::array<int, 2>> moves;
    const int dx[] = {1, 1, -1, -1};
    const int dy[] = {1, -1, 1, -1};
    
    for (int i = 0; i < 4; ++i) {
        for (int step = 1; step < 8; ++step) {
            int nx = position.x + step * dx[i];
            int ny = position.y + step * dy[i];
            
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;
            
            const Piece* p = board.getPieceFromPos({nx, ny});
            if (p == nullptr) {
                moves.push_back({nx, ny});
            } else {
                if (p->getColor() != this->getColor()) {
                    moves.push_back({nx, ny});
                }
                break;
            }
        }
    }
    return moves; 
}