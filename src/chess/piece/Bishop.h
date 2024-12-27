#pragma once

#include "Piece.h"

namespace chess
{
    class Bishop : public Piece
    {
    public:
        Bishop(Board& board, const Point& position, Player& player);

        virtual MoveResult validateMove(const Point& destination) const override;
        virtual char marker() const override;
        virtual int value() const override;
        virtual PieceType getType() const override;
    };
}