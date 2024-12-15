#pragma once

#include <vector>
#include "./piece/Piece.h"

namespace chess
{
    class Player
    {
    public:
        Player(Board& const board);
        ~Player();

        bool mayPerformCastling() const;
        void disallowCastling();

        Board& getBoard() const;

        void devour(const Piece* const piece);
        const std::vector<const Piece*> getDevouredPieces() const;

        int getPointsTotal() const;

    private:
        Board& const _board;

        std::vector<const Piece*> _devouredPieces;
        bool _mayPerformCastling;
    };
}