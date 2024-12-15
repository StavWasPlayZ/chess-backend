#pragma once

#include "../Board.h"
#include "../util/Point.h"
#include "../util/MoveResult.h"
#include "../Player.h"

namespace chess
{
    class Piece
    {
    public:
        Piece(Board& const board, const Point& const position, Player& const player);
        virtual ~Piece();

        virtual MoveResult validateMove(const Point& const destination) const;
        virtual char marker() const = 0;
        virtual int value() const = 0;

        Player& getPlayer() const;
        /**
         * Position may be null after being taken off-board.
         * Returns: The position of this piece on the board.
         */
        const Point* getPosition() const;
        Board* getBoard() const;

        bool isOnBoard() const;

        void onRemovedFromBoard();

    private:
        Player& const _player;
        Board* _board;
        /**
         * Position may be null after being taken off-board.
         */
        Point* _pos;
    };
}