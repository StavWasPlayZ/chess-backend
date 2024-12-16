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
    if (*this->getPosition() == destination)
    {
        return MoveResult::SAME_PLACE;
    }
    
    const Piece* const destPiece = getBoard()->getPieceAt(destination);
    if ((destPiece != nullptr) && (destPiece->getPlayer() == this->getPlayer()))
    {
        return MoveResult::FRIENDLY_FIRE;
    }

    return MoveResult::VALID;
}

void chess::Piece::setPosition(const Point &const point)
{
    *_pos = Point(point);
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
    delete _pos;
    this->_pos = nullptr;
}
