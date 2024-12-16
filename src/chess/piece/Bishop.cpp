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
    
    const Point displacement = getPosition()->displacementFrom(destination);

    // May only move along one axis
    if (displacement.x != 0 && displacement.y != 0)
        return MoveResult::ILLEGAL_MOVE;

    if (displacement.x != 0)
    {
        int normal = (displacement.x > 0) ? 1 : -1;

        if (isInterferenceInRoute(Point(normal, 0), abs(displacement.x)))
        {
            return MoveResult::ILLEGAL_MOVE;
        }

        return MoveResult::LEGAL_MOVE;
    }
    else
    {
        int normal = (displacement.y > 0) ? 1 : -1;

        if (isInterferenceInRoute(Point(0, normal), abs(displacement.y)))
        {
            return MoveResult::ILLEGAL_MOVE;
        }

        return MoveResult::LEGAL_MOVE;
    }

    return MoveResult::LEGAL_MOVE;
}

char chess::Bishop::marker() const
{
    return asPlayerMarker('b');
}

int chess::Bishop::value() const
{
    return 9;
}
