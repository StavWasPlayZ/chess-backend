#pragma once

#include "./piece/Piece.h"
#include "Player.h"
#include <string>

namespace chess
{
    class Board
    {
    public:
        static constexpr int BOARD_SIZE = 8;
        Board();
        ~Board();

        MoveResult movePiece(const std::string& const moveStr);
        MoveResult movePiece(const Point& const source, const Point& const destination);

        bool devourPieceAt(const Point& const point);

        const Piece* getPieceAt(const Point& point) const;

    private:
        Piece* _pieces[BOARD_SIZE][BOARD_SIZE];

        /**
         * Populates the board with a new game.
         * Assumes all preceding resources of this class were freed.
         */
        void populateBoard();

        /**
         * Frees the contents of this board.
         */
        void freeBoard();

        enum class RowPopuplationType
        {
            NONE, PAWNS, ALL_ELSE
        };
        RowPopuplationType getRowPopulationType(const int row) const;


        /**
         * 0 = white, 1 = black
         */
        Player _players[2];
        size_t _playerTurn;
    };
}
