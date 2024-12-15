#pragma once

#include "Piece.h"

namespace chess
{
    class Pawn : public Piece
    {
    public:
        Pawn(Board& const board, const Point& const position);

        virtual MoveResult validateMove(const Point& const point) const;
        virtual char marker() const;
        virtual int value() const;

    private:
        /**
         * Used to determine the first double-jump
        */
        bool wasMoved;
    };
}