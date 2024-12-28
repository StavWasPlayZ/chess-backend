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

        virtual void onMoved(const MoveResult moveResult, const Point& source, const Piece* const devouredPiece) override;
        virtual void onBoardUpdated(const BoardOperationType operationType) override;

    private:
        /**
         * Whether En Passant can be applied to this pawn
         */
        bool _mayEnPassant;
        bool _waitingOnEnPassant;

        /**
         * Returns the pawn to be devoured by En Passant, if any.
         * Input: The position on which to check for En Passant at.
         */
        Pawn* _getEnPassantPiece(const Point& point) const;
        /**
         * Just returns the En Passant piece at the given point.
         * No further checks.
         */
        Pawn* _getEnPassantPieceUnsafe(const Point& point) const;

        /**
         * Returns the position of the En Passant piece from
         * the given position.
         */
        Point _getEnPassantPos(const Point source) const;
    };
}