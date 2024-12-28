#pragma once

#include "../Board.h"
#include "../util/Point.h"
#include "../util/MoveResult.h"
#include "../Player.h"

namespace chess
{
    class Board;
    enum class PieceType;

    class Piece
    {
    public:
        static Piece* fromType(const PieceType type, Board& board, const Point& position, Player& player);

        Piece(Board& board, const Point& position, Player& player);
        virtual ~Piece();

        virtual MoveResult validateMove(const Point& destination) const;
        virtual char marker() const = 0;
        /**
         * The value of this piece in points
         */
        virtual int value() const = 0;
        virtual PieceType getType() const = 0;

        void setPosition(const Point& point);

        Player& getPlayer() const;
        /**
         * Position may be null after being taken off-board.
         * Returns: The position of this piece on the board.
         */
        const Point* getPosition() const;
        Board* getBoard() const;

        bool isOnBoard() const;

        virtual void onMoved(const Piece* const devouredPiece);
        void onRemovedFromBoard();
        void onAddedToBoard();

    protected:
        char asPlayerMarker(const char marker) const;
        /**
         * Checks if any pieces are to be found on the board
         * from this piece's position to the specified goal (exclusive).
         */
        bool isInterferenceInRoute(const Point& direction, const int length) const;

        MoveResult validateOrthogonalMovement(const Point& destination) const;
        MoveResult validateDiagonalMovement(const Point& destination) const;

    private:
        Player& _player;
        Board* _board;
        /**
         * Position may be null after being taken off-board.
         */
        Point* _pos;
    };

    enum class PieceType
    {
        BISHOP,
        KING,
        QUEEN,
        ROOK,
        KNIGHT,
        PAWN
    };
}