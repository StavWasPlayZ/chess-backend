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

        ~Board();

        MoveResult movePiece(const std::string& const moveStr);
        MoveResult movePiece(const Point& const source, const Point& const destination);

        bool devourPieceAt(const Point& const point);

        const Piece* getPieceAt(const Point& point) const;

    private:
        Piece* _pieces[BOARD_SIZE][BOARD_SIZE];

        Player players[2];
        int playerTurn;
    };
}
