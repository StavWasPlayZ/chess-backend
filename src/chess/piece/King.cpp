#include "King.h"
#include <limits.h>

using namespace chess;

chess::King::King(Board &board, const Point &position, Player &player) :
    Piece(board, position, player)
{}

MoveResult chess::King::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    const Point displacement = getPosition()->displacementFrom(destination);
    const int len1 = abs(displacement.x), len2 = abs(displacement.y);

    if ((len1 <= 1) && (len2 <= 1))
        return MoveResult::LEGAL_MOVE;

    //NOTE: Castling was almost implemented, but
    // there was a shortage of time in the end.
    // // Could be trying to perform Castling
    // if (_mayPerformCastling(destination))
    //     return MoveResult::CASTLING;

    return MoveResult::ILLEGAL_MOVE;
}

char chess::King::marker() const
{
    return asPlayerMarker('k');
}

int chess::King::value() const
{
    return INT_MAX;
}

PieceType chess::King::getType() const
{
    return PieceType::KING;
}

void chess::King::onMoved(const MoveResult moveResult, const Point &source, const Piece *const devouredPiece)
{
    // if (moveResult == MoveResult::CASTLING)
    // {

    // }

    Piece::onMoved(moveResult, source, devouredPiece);
}

bool chess::King::_mayPerformCastling(const Point& destination) const
{
    const Point displacement = getPosition()->displacementFrom(destination);

    if (displacement.y != 0)
        return false;

    const Piece* const piece = getBoard()->getPieceAt(destination);
    if (piece == nullptr)
        return false;

    if (piece->getType() != PieceType::ROOK)
        return false;

    // Displacement X can either be negative for left
    // or positive for right. Hence direction
    // vector's polarity doen't really matter.
    if (isInterferenceInRoute(Point(0, 1), displacement.x))
        return false;

    return true;
}
