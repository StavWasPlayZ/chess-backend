#include "Player.h"

using namespace chess;

chess::Player::Player(Board& board, const int number) :
    _mayPerformCastling(true), _board(board), number(number)
{}

chess::Player::~Player()
{
    for (size_t i = 0; i < _devouredPieces.size(); i++)
    {
        delete _devouredPieces[i];
    }
}

bool chess::Player::mayPerformCastling() const
{
    return this->_mayPerformCastling;
}

void chess::Player::disallowCastling()
{
    this->_mayPerformCastling = false;
}

Board &chess::Player::getBoard() const
{
    return this->_board;
}

void chess::Player::devour(const Piece *const piece)
{
    this->_devouredPieces.push_back(piece);
    _board.removePiece(*piece);
}

const std::vector<const Piece *> chess::Player::getDevouredPieces() const
{
    return this->_devouredPieces;
}

int chess::Player::getPointsTotal() const
{
    int sum = 0;

    for (size_t i = 0; i < getDevouredPieces().size(); i++)
    {
        sum += getDevouredPieces()[i]->value();
    }

    return sum;
}

bool chess::Player::operator==(const Player &other) const
{
    return this->number == other.number;
}
