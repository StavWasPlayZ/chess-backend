#include "Piece.h"

using namespace chess;

chess::Piece::Piece(Board &const board, const Point &const position, Player& const player) :
    _board(&board), _pos(new Point(position)), _player(player)
{}

chess::Piece::~Piece()
{
    delete _pos;
}

MoveResult chess::Piece::validateMove(const Point &const destination) const
{
    if (destination.isOutOfBounds())
    {
        return MoveResult::OUT_OF_BOUNDS;
    }

    return MoveResult::VALID;
}

Player &chess::Piece::getPlayer() const
{
    return this->_player;
}

const Point *chess::Piece::getPosition() const
{
    return this->_pos;
}

Board *chess::Piece::getBoard() const
{
    return this->_board;
}

bool chess::Piece::isOnBoard() const
{
    return getBoard() != nullptr;
}

void chess::Piece::onRemovedFromBoard()
{
    this->_board = nullptr;
    this->_pos = nullptr;
}
