#include "Board.h"

using namespace chess;

#include "./piece/Pawn.h"
#include "./piece/Rook.h"
#include "./piece/Knight.h"
#include "./piece/Bishop.h"
#include "./piece/Queen.h"
#include "./piece/King.h"

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

MoveResult chess::Board::movePiece(const Point &const source, const Point &const destination)
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

    if (!isLegal(res))
        return res;
    
    
    const Piece* const overPiece = getPieceAt(destination);

    if (overPiece != nullptr)
    {
        getPlayingPlayer().devour(overPiece);
    }

    piece->setPosition(destination);
    this->_pieces[destination.y][destination.x] = piece;
}

bool chess::Board::removePieceAt(const Point &const point)
{
    if (!hasPieceAt(point))
        return false;
    
    this->_pieces[point.y][point.x] = nullptr;
    return true;
}

bool chess::Board::removePiece(const Piece &const piece)
{
    if (!piece.isOnBoard())
        return false;
    
    removePieceAt(*(piece.getPosition()));
    return true;
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

            Player& const player = this->_players[(i < (BOARD_SIZE / 2)) ? 1 : 0];

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
                piece = new Queen(*this, Point(j, i), player);
            }
            else if (j == 4)
            {
                piece = new Queen(*this, Point(j, i), player);
            }
            // Should be no more.

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
