#pragma once

#include "../piece/Piece.h"

namespace chess
{
    enum class MoveResult
    {
        VALID,
        CHECK,
        
        // Errors
        NO_TOOL,
        FRIENDLY_FIRE, // best naming ive ever done fr
        SELF_CHECK,
        OUT_OF_BOUNDS,
        ILLEGAL_MOVE,
        SAME_PLACE,

        CHECKMATE
    };

    bool isLegal(const MoveResult MoveResult);
}