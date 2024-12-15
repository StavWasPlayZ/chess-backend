#include "Piece.h"

using namespace chess;

chess::Piece::Piece(Board &const board, const Point &const position) :
    _board(&board), _pos(new Point(position))
{}

chess::Piece::~Piece()
{
    delete _pos;
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
