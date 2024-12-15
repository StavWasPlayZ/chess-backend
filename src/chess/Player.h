#pragma once

#include <vector>
#include "./piece/Piece.h"

namespace chess
{
    class Player
    {
    public:
        bool mayPerformCastling;
        
    private:
        std::vector<const Piece&> devouredPieces;
    };
}