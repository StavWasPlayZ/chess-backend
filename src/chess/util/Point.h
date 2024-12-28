#pragma once
#include <string>

namespace chess
{
    struct Point
    {
        Point(const int x, const int y);
        static Point fromChessNotation(const std::string& chessNotation);
        int x, y;

        /**
         * Whether this point exceeds
         * the bounds of a chessboard.
         */
        bool isOutOfBounds() const;

        Point displacementFrom(const Point& other) const;
        Point normalize() const;


        Point operator+(const Point& other) const;
        Point operator*(const Point& other) const;
        Point operator*(const int other) const;

        Point& operator=(const Point& other);
        Point& operator+=(const Point& other);

        bool operator==(const Point& other) const;
        

        std::string toChessNotation() const;
    };
}