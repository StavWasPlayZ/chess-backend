#pragma once

#include "./piece/Piece.h"
#include "Player.h"
#include <string>

namespace chess
{
    class Player;
    class Piece;
    class Pawn;

    struct Point;
    enum class MoveResult;
    enum class PieceType;
    enum class BoardOperationType;

    class Board
    {
    public:
        static constexpr int BOARD_SIZE = 8;
        Board();
        ~Board();

        MoveResult movePiece(const std::string& moveStr, const Player& initiator);
        MoveResult movePiece(const Point& source, const Point& destination, const Player& initiator);

        /**
         * Returns: Whether the board is awaiting for a pawn
         * to be switched.
         */
        bool isInSwitchPawnState() const;
        /**
         * Switches the previously unmovable pawn to
         * a different piece type.
         */
        MoveResult switchPawnTo(const PieceType pieceType, Player& initiator);
        

        /**
         * Removes--but not frees--the piece at the given point.
         * Its stored position will remain unchanged.
         * Returns: Whether there was such a piece to begin with.
         */
        bool removePieceAt(const Point& point);
        /**
         * Removes--but not frees--the provided piece.
         * Its stored position will remain unchanged.
         * Returns: Whether there was such a piece to begin with.
         */
        bool removePiece(const Piece& piece);

        /**
         * Returns: The player that has a check status.
         * Null for none.
         */
        Player* getCheckPlayer() const;
        /**
         * Simlautes the king escaping a check, and if failes -
         * determines a checkmate.
         * Returns: The player that has a checkmate status.
         * Null for none.
         * Input: The player nominated for a checkmate
         */
        Player* getCheckmatePlayer(Player& checkPlayer);

        Piece* const (&getPieces() const)[BOARD_SIZE][BOARD_SIZE];


        Piece* getPieceAt(const Point& point) const;
        bool hasPieceAt(const Point& point) const;

        Player& getPlayer(const int index);
        Player& getPlayingPlayer();
        /**
         * Returns: The turn of the current player.
         * * 0 = white, 1 = black.
         */
        int getPlayerTurn() const;

        /**
         * Returns a string representing this board
         * as a string of all pieces' markers (Piece::marker).
         */
        std::string asPiecesString() const;

    private:
        static constexpr int _PAWN_SWITCHABLES_SIZE = 4;
        static const PieceType _PAWN_SWITCHABLES[_PAWN_SWITCHABLES_SIZE];

        Piece* _pieces[BOARD_SIZE][BOARD_SIZE];
        /**
         * A pawn that is waiting to be replaced by another chess piece.
         * Null for none.
         */
        Pawn* _switchingPawn;

        bool _isValidPawnSwitch(const PieceType pieceType) const;


        /**
         * Checks if the current player is in possession
         * of a check/checkmate.
         * If so, replaces `currRes` with
         * either `MoveResult::CHECK` or `MoveResult::CHECKMATE` alike.
         * Params: The currect result to be switched (conditionally)
         * and the player to check against.
         */
        void _acknowledgeCheckResult(MoveResult& currRes, const Player& player);
        /**
         * Same as the original overload, but provided the checkPlayer was already determined.
         */
        void _acknowledgeCheckResult(MoveResult& currRes, const Player& player, Player* const checkPlayer);


        void _setPieceAt(const Point& pos, Piece& piece);
        /**
         * Moves the piece within the Board::_pieces array.
         */
        void _moveInternal(Piece& piece, const Point pos);

        /**
         * Populates the board with a new game.
         * Assumes all preceding resources of this class were freed.
         */
        void _populateBoard();

        /**
         * Frees the contents of this board.
         */
        void _freeBoard();

        void _notifyBoardUpdate(const BoardOperationType operationType);


        enum class RowPopuplationType
        {
            NONE, PAWNS, ALL_ELSE
        };

        RowPopuplationType _getRowPopulationType(const int row) const;
        PieceType _pieceTypeByColumn(const int column) const;


        /**
         * Must only contain 2 players:
         * 0 = white, 1 = black.
         */
        Player* _players[2];
        size_t _playerTurn;
    };

    enum class BoardOperationType
    {
        MOVE,
        PAWN_SWAP
    };
}
