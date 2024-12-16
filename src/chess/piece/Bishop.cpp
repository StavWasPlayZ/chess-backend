#include "Bishop.h"

using namespace chess;

chess::Bishop::Bishop(Board &board, const Point &position, Player &player) :
    Piece(board, position, player)
{}

MoveResult chess::Bishop::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    return validateDiagonalMovement(destination);
}

char chess::Bishop::marker() const
{
    return asPlayerMarker('b');
}

int chess::Bishop::value() const
{
    return 3;
}
