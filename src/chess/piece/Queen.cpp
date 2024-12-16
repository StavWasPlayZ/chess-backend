#include "Queen.h"

using namespace chess;

chess::Queen::Queen(Board &board, const Point &position, Player &player) :
    Piece(board, position, player)
{}

MoveResult chess::Queen::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    const MoveResult diagonalRes = validateDiagonalMovement(destination);
    if (util::isLegal(diagonalRes))
        return diagonalRes;

    return validateOrthogonalMovement(destination);
}

char chess::Queen::marker() const
{
    return asPlayerMarker('q');
}

int chess::Queen::value() const
{
    return 9;
}
