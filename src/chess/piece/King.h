#pragma once

#include "Piece.h"

namespace chess
{
    class King : public Piece
    {
    public:
        King(Board& board, const Point& position, Player& player);

        virtual MoveResult validateMove(const Point& destination) const override;
        virtual char marker() const override;
        virtual int value() const override;
        virtual PieceType getType() const override;
        virtual void onMoved(const MoveResult moveResult, const Point& source, const Piece* const devouredPiece) override;

    private:
        bool _mayPerformCastling(const Point& destination) const;
    };
}