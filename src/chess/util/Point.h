#pragma once
#include <string>

namespace chess
{
    struct Point
    {
        Point(const int x, const int y);
        int x, y;

        std::string toChessNotation() const;
    };
}