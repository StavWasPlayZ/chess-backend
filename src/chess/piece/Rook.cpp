#include "Rook.h"

using namespace chess;

chess::Rook::Rook(Board &board, const Point &position, Player &player) :
    Piece(board, position, player)
{}

MoveResult chess::Rook::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    return validateOrthogonalMovement(destination);
}

char chess::Rook::marker() const
{
    return asPlayerMarker('r');
}

int chess::Rook::value() const
{
    return 5;
}

PieceType chess::Rook::getType() const
{
    return PieceType::ROOK;
}
