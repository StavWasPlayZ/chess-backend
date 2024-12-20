#include "Knight.h"

using namespace chess;

chess::Knight::Knight(Board &board, const Point &position, Player &player) :
    Piece(board, position, player)
{}

MoveResult chess::Knight::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    const Point displacement = getPosition()->displacementFrom(destination);
    const int len1 = abs(displacement.x), len2 = abs(displacement.y);

    if (
           ((len1 == 1) && (len2 == 2))
        || ((len1 == 2) && (len2 == 1))
    )
        return MoveResult::LEGAL_MOVE;

    return MoveResult::ILLEGAL_MOVE;
}

char chess::Knight::marker() const
{
    return asPlayerMarker('q');
}

int chess::Knight::value() const
{
    return 9;
}
