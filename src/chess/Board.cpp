#include "Board.h"

using namespace chess;

#include "./piece/Pawn.h"
#include "./piece/Rook.h"
#include "./piece/Knight.h"
#include "./piece/Bishop.h"
#include "./piece/Queen.h"
#include "./piece/King.h"

#include <sstream>

/**
 * Checks if the goal was reached from either the
 * beggining or end of an array.
 */
static bool mirrorCheck(int index, int goal, int size = Board::BOARD_SIZE)
{
    return (index == goal) || ((index + 1) == (size - goal));
}

chess::Board::Board() :
    _playerTurn(0)
{
    this->_players.reserve(2);
    for (size_t i = 0; i < 2; i++)
    {
        this->_players.push_back(Player(*this, i));
    }

    _populateBoard();
}

chess::Board::~Board()
{
    _freeBoard();
}

MoveResult chess::Board::movePiece(const std::string &moveStr)
{
    return movePiece(
        Point::fromChessNotation(moveStr.substr(0, 2)),
        Point::fromChessNotation(moveStr.substr(2, 4))
    );
}

MoveResult chess::Board::movePiece(const Point &source, const Point &destination)
{
    if (source.isOutOfBounds() || destination.isOutOfBounds())
    {
        return MoveResult::OUT_OF_BOUNDS;
    }

    Piece* const piece = getPieceAt(source);
    if (piece == nullptr)
    {
        return MoveResult::NO_TOOL;
    }

    MoveResult res = piece->validateMove(destination);

    if (!util::isLegal(res))
        return res;
    
    
    Piece* const overPiece = getPieceAt(destination);

    removePieceAt(source);
    _setPieceAt(destination, *piece);

    // Check for self-check
    const Player* const checkPlayer = getCheckPlayer();
    const Player& thisPlayer = piece->getPlayer();

    if (thisPlayer.getOther() == *checkPlayer)
    {
        // Revert board
        _setPieceAt(source, *piece);
        _setPieceAt(destination, *overPiece);

        //NOTE: Re-do check statuses if necessary

        return MoveResult::SELF_CHECK;
    }

    // There was already a piece at the destination
    if (overPiece != nullptr)
    {
        getPlayingPlayer().devour(overPiece);
    }

    piece->onMoved(overPiece);

    // Check check status (for result)
    // Can only be this player (per last check)
    if (checkPlayer != nullptr)
    {
        if (thisPlayer == *getCheckmatePlayer())
            return MoveResult::CHECKMATE;

        return MoveResult::CHECK;
    }

    return res;
}

bool chess::Board::removePieceAt(const Point &point)
{
    if (!hasPieceAt(point))
        return false;
    
    this->_pieces[point.y][point.x] = nullptr;
    return true;
}

bool chess::Board::removePiece(const Piece &piece)
{
    if (!piece.isOnBoard())
        return false;
    
    removePieceAt(*(piece.getPosition()));
    return true;
}

Player *chess::Board::getCheckPlayer() const
{
    return nullptr;
}

Player *chess::Board::getCheckmatePlayer() const
{
    return nullptr;
}

Piece* const (&chess::Board::getPieces() const)[Board::BOARD_SIZE][Board::BOARD_SIZE]
{
    return this->_pieces;
}

Piece *chess::Board::getPieceAt(const Point &point) const
{
    return this->_pieces[point.y][point.x];
}

bool chess::Board::hasPieceAt(const Point &point) const
{
    return getPieceAt(point) != nullptr;
}

Player &chess::Board::getPlayer(const int index)
{
    return this->_players[index];
}

Player &chess::Board::getPlayingPlayer()
{
    return getPlayer(getPlayerTurn());
}

int chess::Board::getPlayerTurn() const
{
    return this->_playerTurn;
}

std::string chess::Board::asPiecesString() const
{
    std::ostringstream builder;

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            const Piece* const piece = getPieces()[i][j];
            builder << ((piece == nullptr)
                ? '#'
                : piece->marker()
            );
        }
    }

    return builder.str();
}

void chess::Board::_setPieceAt(const Point &pos, Piece &piece)
{
    this->_pieces[pos.y][pos.x] = &piece;
    piece.setPosition(pos);
}

void chess::Board::_populateBoard()
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        RowPopuplationType populationType = _getRowPopulationType(i);

        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            if (populationType == RowPopuplationType::NONE)
            {
                this->_pieces[i][j] = nullptr;
                continue;
            }

            Player& player = this->_players[(i < (BOARD_SIZE / 2)) ? 1 : 0];

            if (populationType == RowPopuplationType::PAWNS)
            {
                this->_pieces[i][j] = new Pawn(*this, Point(j, i), player);
                continue;
            }

            Piece* piece;

            if (mirrorCheck(j, 0))
            {
                piece = new Rook(*this, Point(j, i), player);
            }
            else if (mirrorCheck(j, 1))
            {
                piece = new Knight(*this, Point(j, i), player);
            }
            else if (mirrorCheck(j, 2))
            {
                piece = new Bishop(*this, Point(j, i), player);
            }
            else if (j == 3)
            {
                piece = new King(*this, Point(j, i), player);
            }
            else if (j == 4)
            {
                piece = new Queen(*this, Point(j, i), player);
            }
            else
            {
                throw std::logic_error("Invalid piece type");
            }

            this->_pieces[i][j] = piece;
        }
    }
}

void chess::Board::_freeBoard()
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            delete this->_pieces[i][j];
            this->_pieces[i][j] = nullptr;
        }
    }
}

Board::RowPopuplationType chess::Board::_getRowPopulationType(const int row) const
{
    return
        mirrorCheck(row, 0)
            ? RowPopuplationType::ALL_ELSE
        : mirrorCheck(row, 1)
            ? RowPopuplationType::PAWNS
        : RowPopuplationType::NONE;
}
