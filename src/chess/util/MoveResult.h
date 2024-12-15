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
        INVALID_INDEXES,
        ILLEGAL_MOVE,
        SAME_PLACE,

        CHECKMATE
    };

    bool isLegal(const MoveResult MoveResult);
    bool isIllegal(const MoveResult MoveResult);
}