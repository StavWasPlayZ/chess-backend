#pragma once

#include "../Board.h"

namespace chess
{
    namespace util
    {
        /**
         * Checks if the goal was reached from either the
         * beggining or end of an array.
         */
        bool mirrorCheck(int index, int goal, int size = Board::BOARD_SIZE);
    }
}