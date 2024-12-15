#pragma once

#include "./piece/Piece.h"
#include "Player.h"
#include <string>

namespace chess
{
    class Board
    {
    public:
        static constexpr int BOARD_SIZE = 8;
        Board();
        ~Board();

        MoveResult movePiece(const std::string& const moveStr);
        MoveResult movePiece(const Point& const source, const Point& const destination);

        /**
         * Removes--but not frees--the piece at the given point.
         * Returns: Whether there was such a piece to begin with.
         */
        bool removePieceAt(const Point& const point);
        /**
         * Removes--but not frees--the provided piece.
         * Returns: Whether there was such a piece to begin with.
         */
        bool removePiece(const Piece& const piece);


        Piece* getPieceAt(const Point& point) const;
        bool hasPieceAt(const Point& point) const;

        Player& getPlayer(const int index);
        Player& getPlayingPlayer();
        /**
         * Returns: The turn of the current player.
         * * 0 = white, 1 = black.
         */
        int getPlayerTurn() const;

    private:
        Piece* _pieces[BOARD_SIZE][BOARD_SIZE];

        /**
         * Populates the board with a new game.
         * Assumes all preceding resources of this class were freed.
         */
        void _populateBoard();

        /**
         * Frees the contents of this board.
         */
        void _freeBoard();

        enum class RowPopuplationType
        {
            NONE, PAWNS, ALL_ELSE
        };
        RowPopuplationType _getRowPopulationType(const int row) const;


        /**
         * Must only contain 2 players:
         * 0 = white, 1 = black.
         * This is not an array becase Player has no default constructor.
         */
        std::vector<Player> _players;
        size_t _playerTurn;
    };
}
