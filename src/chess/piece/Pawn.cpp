#include "Pawn.h"

using namespace chess;

chess::Pawn::Pawn(Board &board, const Point &position, Player &player) :
    Piece(board, position, player), wasMoved(false)
{}

MoveResult chess::Pawn::validateMove(const Point &destination) const
{
    {
        const MoveResult moveRes = Piece::validateMove(destination);
        if (!util::isLegal(moveRes))
            return moveRes;
    }
    
    const Point displacement = getPosition()->displacementFrom(destination);

    if (
        // White may only go up; vice versa.
        (getPlayer().number == 0)
            ? (displacement.y >= 0)
            : (displacement.y <= 0)
    )
        return MoveResult::ILLEGAL_MOVE;

    if (abs(displacement.y) == 1)
    {
        if (displacement.x == 0)
        {
            return MoveResult::LEGAL_MOVE;
        }
        else
        {
            // Special eating move; diagonal
            if (abs(displacement.x) != 1)
                return MoveResult::ILLEGAL_MOVE;

            // Must be eating the *opposing* player
            const Piece* const piece = getBoard()->getPieceAt(destination);
            if ((piece == nullptr) || (piece->getPlayer() == getPlayer()))
                return MoveResult::ILLEGAL_MOVE;

            return MoveResult::LEGAL_MOVE;
        }
    }

    if (abs(displacement.y) == 2)
    {
        if (wasMoved || (displacement.x != 0))
            return MoveResult::ILLEGAL_MOVE;

        return MoveResult::LEGAL_MOVE;
    }

    return MoveResult::ILLEGAL_MOVE;
}

char chess::Pawn::marker() const
{
    return asPlayerMarker('p');
}

int chess::Pawn::value() const
{
    return 1;
}

void chess::Pawn::onMoved(const Piece *const devouredPiece)
{
    this->wasMoved = true;
}
