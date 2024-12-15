#pragma once

#include "Piece.h"

namespace chess
{
    class King : public Piece
    {
    public:
        King(Board& const board, const Point& const position);

        virtual MoveResult validateMove(const Point& const point) const;
        virtual char marker() const;
        virtual int value() const;
    };
}