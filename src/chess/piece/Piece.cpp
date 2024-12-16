#include "Piece.h"

using namespace chess;

chess::Piece::Piece(Board &board, const Point &position, Player& player) :
    _board(&board), _pos(new Point(position)), _player(player)
{}

chess::Piece::~Piece()
{
    delete _pos;
}

MoveResult chess::Piece::validateMove(const Point &destination) const
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

    return MoveResult::LEGAL_MOVE;
}

void chess::Piece::setPosition(const Point &point)
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

void chess::Piece::onMoved(const Piece* const devouredPiece) {}

void chess::Piece::onRemovedFromBoard()
{
    this->_board = nullptr;
    delete _pos;
    this->_pos = nullptr;
}
