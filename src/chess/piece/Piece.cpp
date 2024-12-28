#include "Piece.h"

#include <stdexcept>

#include "./Pawn.h"
#include "./Rook.h"
#include "./Knight.h"
#include "./Bishop.h"
#include "./Queen.h"
#include "./King.h"

using namespace chess;

Piece* chess::Piece::fromType(const PieceType type, Board &board, const Point &position, Player &player)
{
    switch (type)
    {
        case PieceType::PAWN: return new Pawn(board, position, player);
        case PieceType::ROOK: return new Rook(board, position, player);
        case PieceType::KNIGHT: return new Knight(board, position, player);
        case PieceType::KING: return new King(board, position, player);
        case PieceType::BISHOP: return new Bishop(board, position, player);
        case PieceType::QUEEN: return new Queen(board, position, player);
    }

    throw std::invalid_argument("Invalid chess piece type");
}

chess::Piece::Piece(Board &board, const Point &position, Player &player) :
    _board(&board),
    _pos(new Point(position)),
    _player(player),
    _wasMoved(false)
{}

chess::Piece::~Piece()
{
    delete _pos;
}

MoveResult chess::Piece::validateMove(const Point &destination) const
{
    if (*this->getPosition() == destination)
        return MoveResult::SAME_PLACE;

    if (destination.isOutOfBounds())
        return MoveResult::OUT_OF_BOUNDS;

    const Piece* const destPiece = getBoard()->getPieceAt(destination);

    if ((destPiece != nullptr) && (destPiece->getPlayer() == this->getPlayer()))
        return MoveResult::FRIENDLY_FIRE;

    //NOTE: The king check is done only at move time.
    // This is because validateMove is also used to check
    // for a check (moving to king).

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

bool chess::Piece::wasMoved() const
{
    return this->_wasMoved;
}

bool chess::Piece::isOnBoard() const
{
    return getBoard() != nullptr;
}

void chess::Piece::onMoved(const Point& source, const Piece* const devouredPiece)
{
    this->_wasMoved = true;
}

void chess::Piece::onRemovedFromBoard()
{
    getPlayer().removePiece(*this);

    this->_board = nullptr;
    delete _pos;
    this->_pos = nullptr;
}

void chess::Piece::onAddedToBoard()
{
    _player.addPiece(*this);
}

void chess::Piece::onBoardUpdated(const BoardOperationType operationType) {}

char chess::Piece::asPlayerMarker(const char marker) const
{
    return (getPlayer().number == 0) ? marker : toupper(marker);
}

bool chess::Piece::isInterferenceInRoute(const Point &direction, const int length) const
{
    for (size_t i = 1; i < length; i++)
    {
        if (getBoard()->getPieceAt(*getPosition() + direction * i) != nullptr)
            return true;
    }

    return false;
}


MoveResult chess::Piece::validateOrthogonalMovement(const Point &destination) const
{
    const Point displacement = getPosition()->displacementFrom(destination);

    // May only move along 1 axis
    if ((displacement.x != 0) && (displacement.y != 0))
        return MoveResult::ILLEGAL_MOVE;

    const int length = abs((displacement.x == 0) ? displacement.y : displacement.x);

    if (isInterferenceInRoute(displacement.normalize(), length))
        return MoveResult::ILLEGAL_MOVE;

    return MoveResult::LEGAL_MOVE;
}

MoveResult chess::Piece::validateDiagonalMovement(const Point &destination) const
{
    const Point displacement = getPosition()->displacementFrom(destination);
    const int length = abs(displacement.x);

    // May only move when both axis are of the same value
    if (length != abs(displacement.y))
        return MoveResult::ILLEGAL_MOVE;

    if (isInterferenceInRoute(displacement.normalize(), length))
        return MoveResult::ILLEGAL_MOVE;

    return MoveResult::LEGAL_MOVE;
}
