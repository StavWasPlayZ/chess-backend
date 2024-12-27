#pragma once

#include <vector>
#include "./piece/Piece.h"
#include "Board.h"

namespace chess
{
    class Board;
    class Piece;
    class King;
    
    class Player
    {
    public:
        Player(Board& board, const int number);
        ~Player();

        //TODO change to enum
        /**
         * 0 = white, 1 = black.
         */
        const int number;

        bool mayPerformCastling() const;
        void disallowCastling();

        Board& getBoard() const;

        King& getKing() const;
        void setKing(King& king);


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
        King* _king;

        std::vector<const Piece*> _devouredPieces;
        bool _mayPerformCastling;
    };
}