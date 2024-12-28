#include "Pawn.h"

using namespace chess;

chess::Pawn::Pawn(Board &board, const Point &position, Player &player) :
    Piece(board, position, player),
    _mayEnPassant(false),
    _waitingOnEnPassant(false)
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
        const Piece* const destPiece = getBoard()->getPieceAt(destination);

        if (displacement.x == 0)
        {
            // Must not have any piece at the destination.
            if (destPiece == nullptr)
                return MoveResult::LEGAL_MOVE;

            return MoveResult::ILLEGAL_MOVE;
        }
        else
        {
            // Special eating move; diagonal
            if (abs(displacement.x) != 1)
                return MoveResult::ILLEGAL_MOVE;

            if (destPiece != nullptr)
            {
                // Must be eating the *opposing* player
                if (destPiece->getPlayer() == getPlayer())
                    return MoveResult::ILLEGAL_MOVE;

                return MoveResult::LEGAL_MOVE;
            }

            // Could be En Passant
            if (_getEnPassantPiece(destination) != nullptr)
                return MoveResult::EN_PASSANT;

            return MoveResult::ILLEGAL_MOVE;
        }
    }

    if (abs(displacement.y) == 2)
    {
        if (wasMoved() || (displacement.x != 0))
            return MoveResult::ILLEGAL_MOVE;

        if (isInterferenceInRoute(displacement.normalize(), 3))
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

PieceType chess::Pawn::getType() const
{
    return PieceType::PAWN;
}

void chess::Pawn::onMoved(const MoveResult moveResult, const Point& source, const Piece* const devouredPiece)
{
    if (moveResult == MoveResult::EN_PASSANT)
    {
        Pawn* enPassanted = _getEnPassantPieceUnsafe(*getPosition());
        getBoard()->removePiece(*enPassanted);
        getPlayer().devour(enPassanted);
    }

    // May only perform En Passant if this was our first time moving.
    else if (!wasMoved())
    {
        const Point displacement = source.displacementFrom(*getPosition());
        if (abs(displacement.y) == 2)
        {
            this->_mayEnPassant = true;
        }
    }

    Piece::onMoved(moveResult, source, devouredPiece);
}

void chess::Pawn::onBoardUpdated(const BoardOperationType operationType)
{
    if (operationType != BoardOperationType::MOVE)
    {
        Piece::onBoardUpdated(operationType);
        return;
    }
    
    if (this->_mayEnPassant)
    {
        // We've already waited a whole round -
        // No more en-passant.
        if (_waitingOnEnPassant)
        {
            this->_mayEnPassant = false;
            this->_waitingOnEnPassant = false;
        }
        else
        {
            this->_waitingOnEnPassant = true;
        }
    }

    Piece::onBoardUpdated(operationType);
}

Pawn *chess::Pawn::_getEnPassantPiece(const Point& point) const
{
    const Piece* epPiece = getBoard()->getPieceAt(_getEnPassantPos(point));
    if (epPiece == nullptr)
        return nullptr;
    
    if (epPiece->getType() != PieceType::PAWN)
        return nullptr;

    Pawn* asPawn = (Pawn*) epPiece;

    if (!asPawn->_mayEnPassant)
        return nullptr;

    return asPawn;
}

Pawn *chess::Pawn::_getEnPassantPieceUnsafe(const Point &point) const
{
    return (Pawn*) getBoard()->getPieceAt(_getEnPassantPos(point));
}

Point chess::Pawn::_getEnPassantPos(const Point source) const
{
    return source + (
        (getPlayer().number == 0)
            ? Point(0, 1)
            : Point(0, -1)
    );
}
