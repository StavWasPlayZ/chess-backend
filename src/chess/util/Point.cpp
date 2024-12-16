#include "Point.h"

#include "../Board.h"

using namespace chess;

chess::Point::Point(const int x, const int y) :
    x(x), y(y)
{}

bool chess::Point::isOutOfBounds() const
{
    return (x < 0) || (y < 0)
        || (x >= Board::BOARD_SIZE) || (y >= Board::BOARD_SIZE);
}

Point &chess::Point::operator=(const Point &other)
{
    this->x = other.x;
    this->y = other.y;
}

std::string chess::Point::toChessNotation() const
{
    const char str[] = {
        'a' + (this->x),
        '0' + (Board::BOARD_SIZE - this->y)
    };
    return std::string(str, 2);
}
