#pragma once
#include <string>

namespace chess
{
    struct Point
    {
        Point(const int x, const int y);
        const int x, y;

        /**
         * Whether this point exceeds
         * the bounds of a chess board.
         */
        bool isOutOfBounds() const;

        std::string toChessNotation() const;
    };
}