#pragma once

#include "../Board.h"
#include "../util/Point.h"
#include "../util/MoveResult.h"

namespace chess
{
    class Piece
    {
    public:
        Piece(Board& const board, const Point& const position);
        virtual ~Piece();

        virtual MoveResult validateMove(const Point& const point) const = 0;
        virtual char marker() const = 0;
        virtual int value() const = 0;

        /**
         * Position may be null after being taken off-board.
         * Returns: The position of this piece on the board.
         */
        const Point* getPosition() const;
        Board* getBoard() const;

        void onRemovedFromBoard();

    private:
        Board* _board;
        /**
         * Position may be null after being taken off-board.
         */
        Point* _pos;
    };
}