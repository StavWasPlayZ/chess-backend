#pragma once

#include <vector>
#include "./piece/Piece.h"

namespace chess
{
    class Player
    {
    public:
        ~Player();

        bool mayPerformCastling() const;
        void disallowCastling();

        void devour(const Piece* const piece);
        const std::vector<const Piece*> getDevouredPieces() const;

    private:
        std::vector<const Piece*> _devouredPieces;
        bool _mayPerformCastling;
    };
}