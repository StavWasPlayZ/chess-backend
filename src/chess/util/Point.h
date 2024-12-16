#pragma once
#include <string>

namespace chess
{
    struct Point
    {
        Point(const int x, const int y);
        static Point fromChessNotation(const std::string& const chessNotation);
        int x, y;

        /**
         * Whether this point exceeds
         * the bounds of a chess board.
         */
        bool isOutOfBounds() const;

        Point& operator=(const Point& other);

        std::string toChessNotation() const;
    };
}