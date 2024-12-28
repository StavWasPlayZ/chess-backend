#include "Point.h"

#include "../Board.h"
#include <stdexcept>

using namespace chess;

chess::Point::Point(const int x, const int y) :
    x(x), y(y)
{}

Point chess::Point::fromChessNotation(const std::string &chessNotation)
{
    if (chessNotation.length() != 2)
        throw std::invalid_argument("Invalid chess notation: Chess notation may only be of length 2.");

    if (!(
           ('a' <= chessNotation[0]) && (chessNotation[0] <= 'z')
        && ('0' <= chessNotation[1]) && (chessNotation[1] <= '9')
    ))
        throw std::invalid_argument("Invalid chess notation: String must be a character followed by a number.");

    return Point(chessNotation[0] - 'a', Board::BOARD_SIZE - (int)(chessNotation[1] - '0'));
}

bool chess::Point::isOutOfBounds() const
{
    return (x < 0) || (y < 0)
        || (x >= Board::BOARD_SIZE) || (y >= Board::BOARD_SIZE);
}

Point chess::Point::displacementFrom(const Point &other) const
{
    return Point(
        other.x - this->x,
        other.y - this->y
    );
}

static int normalizeI(const int num)
{
    return (num == 0) ? 0
        : (num > 0) ? 1 : -1;
}

Point chess::Point::normalize() const
{
    return Point(normalizeI(this->x), normalizeI(this->y));
}

Point chess::Point::operator+(const Point &other) const
{
    return Point(this->x + other.x, this->y + other.y);
}

Point chess::Point::operator*(const Point &other) const
{
    return Point(this->x * other.x, this->y * other.y);
}

Point chess::Point::operator*(const int scaler) const
{
    return Point(this->x * scaler, this->y * scaler);
}

Point &chess::Point::operator=(const Point &other)
{
    this->x = other.x;
    this->y = other.y;
    return *this;
}

bool chess::Point::operator==(const Point &other) const
{
    return (this->x == other.x) && (this->y == other.y);
}

Point &chess::Point::operator+=(const Point &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

std::string chess::Point::toChessNotation() const
{
    const char str[] = {
        (char)('a' + (this->x)),
        (char)('0' + (Board::BOARD_SIZE - this->y))
    };
    return std::string(str, 2);
}
