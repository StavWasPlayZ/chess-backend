#pragma once

#include <vector>
#include "./piece/Piece.h"
#include "Board.h"

namespace chess
{
    class Board;
    class Piece;
    
    class Player
    {
    public:
        Player(Board& board, const int number);
        ~Player();

        /**
         * 0 = white, 1 = black.
         */
        const int number;

        bool mayPerformCastling() const;
        void disallowCastling();

        Board& getBoard() const;

        void devour(const Piece* const piece);
        const std::vector<const Piece*> getDevouredPieces() const;

        int getPointsTotal() const;

        bool operator==(const Player& other) const;
        bool operator!=(const Player& other) const;

        /**
         * Gets the other player on the board
         */
        Player& getOther() const;

    private:
        Board& _board;

        std::vector<const Piece*> _devouredPieces;
        bool _mayPerformCastling;
    };
}