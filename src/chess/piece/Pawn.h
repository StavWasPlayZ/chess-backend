#pragma once

#include "Piece.h"

namespace chess
{
    class Pawn : public Piece
    {
    public:
        Pawn(Board& board, const Point& position, Player& player);

        virtual MoveResult validateMove(const Point& destination) const override;
        virtual char marker() const override;
        virtual int value() const override;
        virtual PieceType getType() const override;

        virtual void onMoved(const Point& source, const Piece* const devouredPiece) override;
        virtual void onBoardUpdated(const BoardOperationType operationType) override;

    private:
        /**
         * Whether En Passant can be applied to this pawn
         */
        bool _mayEnPassant;
        bool _waitingOnEnPassant;
    };
}