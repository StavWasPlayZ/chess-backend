#pragma once

#include "Piece.h"

namespace chess
{
    class Bishop : public Piece
    {
    public:
        Bishop(Board& const board, const Point& const position, Player& const player);

        virtual MoveResult validateMove(const Point& const point) const;
        virtual char marker() const;
        virtual int value() const;
    };
}