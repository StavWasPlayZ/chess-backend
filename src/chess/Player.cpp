#include "Player.h"

using namespace chess;

#include <algorithm>

chess::Player::Player(Board& board, const int number) :
    number(number),
    _board(board),
    _mayPerformCastling(true)
{
    _pieces.reserve(_PIECES_PER_PLAYER);
}

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

void chess::Player::addPiece(Piece &piece)
{
    this->_pieces.push_back(&piece);

    if (piece.getType() == PieceType::KING)
    {
        this->_king = (King*) &piece;
    }
}

void chess::Player::removePiece(const Piece &piece)
{
    std::vector<Piece*>::iterator it = std::find(this->_pieces.begin(), this->_pieces.end(), &piece);
    if (it == this->_pieces.end())
        return;

    this->_pieces.erase(it);
}

const std::vector<Piece *> &chess::Player::getPieces() const
{
    return this->_pieces;
}

King &chess::Player::getKing() const
{
    return *this->_king;
}

void chess::Player::devour(Piece *const piece)
{
    this->_devouredPieces.push_back(piece);
    piece->onRemovedFromBoard();
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

bool chess::Player::operator!=(const Player &other) const
{
    return this->number != other.number;
}

Player &chess::Player::getOther() const
{
    return getBoard().getPlayer(1 - this->number);
}
