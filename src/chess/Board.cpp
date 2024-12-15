#include "Board.h"

using namespace chess;

#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

/**
 * Checks if the goal was reached from either the
 * beggining or end of an array.
 */
static bool mirrorCheck(int index, int goal, int size)
{
    return (index == goal) || ((index + 1) == (size - goal));
}

chess::Board::Board() :
    _playerTurn(0)
{
    populateBoard();
}

chess::Board::~Board()
{
    freeBoard();
}

void chess::Board::populateBoard()
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        RowPopuplationType populationType = getRowPopulationType(i);

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

            if (mirrorCheck(j, 0, BOARD_SIZE))
            {
                piece = new Rook(*this, Point(j, i), player);
            }
            else if (mirrorCheck(j, 1, BOARD_SIZE))
            {
                piece = new Knight(*this, Point(j, i), player);
            }
            else if (mirrorCheck(j, 2, BOARD_SIZE))
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

void chess::Board::freeBoard()
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

Board::RowPopuplationType chess::Board::getRowPopulationType(const int row) const
{
    return
        mirrorCheck(row, 0, BOARD_SIZE)
            ? RowPopuplationType::ALL_ELSE
        : mirrorCheck(row, 1, BOARD_SIZE)
            ? RowPopuplationType::PAWNS
        : RowPopuplationType::NONE;
}
