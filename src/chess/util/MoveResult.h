#pragma once

#include "../piece/Piece.h"

namespace chess
{
    struct MoveResult
    {
        MoveResult(const int statusCode, const Piece* const devouredPiece);
        const int statusCode;
        const Piece* const devouredPiece;
    };
}