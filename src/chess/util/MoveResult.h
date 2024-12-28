#pragma once

#include "../piece/Piece.h"

namespace chess
{
    enum class MoveResult
    {
        LEGAL_MOVE,
        CHECK,
        
        // Errors
        NO_TOOL,
        FRIENDLY_FIRE, // best naming ive ever done fr
        SELF_CHECK,
        OUT_OF_BOUNDS,
        ILLEGAL_MOVE,
        SAME_PLACE,

        CHECKMATE,

        // Custom
        EN_PASSANT
    };

    namespace util
    {
        bool isLegal(const MoveResult MoveResult);
    }
}