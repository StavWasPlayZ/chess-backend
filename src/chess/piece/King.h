#pragma once

#include "Piece.h"

namespace chess
{
    class King : public Piece
    {
    public:
        King(Board& const board, const Point& const position, Player& const player);

        virtual MoveResult validateMove(const Point& const destination) const;
        virtual char marker() const;
        virtual int value() const;
    };
}