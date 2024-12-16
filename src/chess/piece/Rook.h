#pragma once

#include "Piece.h"

namespace chess
{
    class Rook : public Piece
    {
    public:
        Rook(Board& board, const Point& position, Player& player);

        virtual MoveResult validateMove(const Point& destination) const;
        virtual char marker() const;
        virtual int value() const;
    };
}