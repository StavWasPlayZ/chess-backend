#pragma once

#include "Piece.h"

namespace chess
{
    class Pawn : public Piece
    {
    public:
        Pawn(Board& board, const Point& position, Player& player);

        virtual MoveResult validateMove(const Point& destination) const;
        virtual char marker() const;
        virtual int value() const;

        virtual void onMoved(const Piece* const devouredPiece) override;

    private:
        /**
         * Used to determine the first double-jump
        */
        bool wasMoved;
    };
}