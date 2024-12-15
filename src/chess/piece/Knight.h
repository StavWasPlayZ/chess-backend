#pragma once

#include "Piece.h"

namespace chess
{
    class Knight : public Piece
    {
    public:
        Knight(Board& const board, const Point& const position, Player& const player);

        virtual MoveResult validateMove(const Point& const destination) const;
        virtual char marker() const;
        virtual int value() const;
    };
}