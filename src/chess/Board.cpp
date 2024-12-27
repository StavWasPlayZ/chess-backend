#include "Board.h"

using namespace chess;

#include "./piece/Pawn.h"

#include <sstream>


const PieceType Board::_PAWN_SWITCHABLES[Board::_PAWN_SWITCHABLES_SIZE] =
{
    PieceType::QUEEN,
    PieceType::ROOK,
    PieceType::BISHOP,
    PieceType::KNIGHT
};


/**
 * Checks if the goal was reached from either the
 * beggining or end of an array.
 */
static bool mirrorCheck(int index, int goal, int size = Board::BOARD_SIZE)
{
    return (index == goal) || ((index + 1) == (size - goal));
}

chess::Board::Board() :
    _playerTurn(0),
    _switchingPawn(nullptr)
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

MoveResult chess::Board::movePiece(const std::string &moveStr, const Player& initiator)
{
    return movePiece(
        Point::fromChessNotation(moveStr.substr(0, 2)),
        Point::fromChessNotation(moveStr.substr(2, 4)),
        initiator
    );
}

MoveResult chess::Board::movePiece(const Point &source, const Point &destination, const Player& initiator)
{
    // There shouldn't be any movement;
    // We're waiting for a pawn switch.
    if (isInSwitchPawnState())
        return MoveResult::ILLEGAL_MOVE;

    if (initiator != getPlayingPlayer())
    {
        //NOTE: A much better approach would be to just omit this field altogether.
        // Both the frontend and backend should already be in sync on this matter.
        //
        // This would, however, be more appropriate for server-client relations,
        // if that was the aim. I guess.
        // Though the identifier would be the socket instance itself, or some session ID,
        // and not some random integer passed on as part of a message.
        return MoveResult::ILLEGAL_MOVE;
    }
    
    if (source.isOutOfBounds() || destination.isOutOfBounds())
        return MoveResult::OUT_OF_BOUNDS;

    Piece* const piece = getPieceAt(source);
    if (piece == nullptr)
        return MoveResult::NO_TOOL;

    MoveResult res = piece->validateMove(destination);

    if (!util::isLegal(res))
        return res;
    
    
    Piece* const overPiece = getPieceAt(destination);

    removePieceAt(source);
    _setPieceAt(destination, *piece);

    // Check for self-check
    const Player* const checkPlayer = getCheckPlayer();
    if ((checkPlayer != nullptr) && (initiator != *checkPlayer))
    {
        // Revert board
        _setPieceAt(source, *piece);
        _setPieceAt(destination, *overPiece);

        return MoveResult::SELF_CHECK;
    }

    // There was already a piece at the destination
    if (overPiece != nullptr)
    {
        getPlayingPlayer().devour(overPiece);
    }

    // If it's a pawn, and it reached the end - it may be replaced.
    if (piece->getType() == PieceType::PAWN)
    {
        int dest = (initiator.number == 0) ? 0 : (BOARD_SIZE - 1);

        if (piece->getPosition()->y == dest)
        {
            this->_switchingPawn = (Pawn*) piece;
        }
    }

    piece->onMoved(overPiece);

    _acknowledgeCheckResult(res, initiator);

    if (!isInSwitchPawnState())
    {
        this->_playerTurn = 1 - this->_playerTurn;
    }
    return res;
}

bool chess::Board::isInSwitchPawnState() const
{
    return this->_switchingPawn != nullptr;
}

MoveResult chess::Board::switchPawnTo(const PieceType pieceType, Player &initiator)
{
    if (!isInSwitchPawnState())
        return MoveResult::ILLEGAL_MOVE;

    if (initiator != getPlayingPlayer())
        return MoveResult::ILLEGAL_MOVE;

    if (!_isValidPawnSwitch(pieceType))
        return MoveResult::ILLEGAL_MOVE;

    const Point& pos = *this->_switchingPawn->getPosition();

    Piece* newPiece = Piece::fromType(pieceType, *this, pos, initiator);
    _setPieceAt(pos, *newPiece);


    // Check for self-check
    const Player* const checkPlayer = getCheckPlayer();
    if ((checkPlayer != nullptr) && (initiator != *checkPlayer))
    {
        // Revert piece
        _setPieceAt(pos, *this->_switchingPawn);
        delete newPiece;

        return MoveResult::SELF_CHECK;
    }


    MoveResult res = MoveResult::LEGAL_MOVE;

    _acknowledgeCheckResult(res, initiator);

    delete this->_switchingPawn;
    this->_switchingPawn = nullptr;

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

bool chess::Board::_isValidPawnSwitch(const PieceType pieceType) const
{
    for (size_t i = 0; i < _PAWN_SWITCHABLES_SIZE; i++)
    {
        if (_PAWN_SWITCHABLES[i] == pieceType)
        {
            return true;
        }
    }

    return false;
}

void chess::Board::_acknowledgeCheckResult(MoveResult &currRes, const Player &player) const
{
    const Player* checkPlayer = getCheckPlayer();
    if (checkPlayer == nullptr)
        return;

    if (player != *checkPlayer)
        return;
    
    if (player == *getCheckmatePlayer())
    {
        currRes = MoveResult::CHECKMATE;
    }
    else
    {
        currRes = MoveResult::CHECK;
    }
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

            PieceType pieceType;

            if (mirrorCheck(j, 0))
            {
                pieceType = PieceType::ROOK;
            }
            else if (mirrorCheck(j, 1))
            {
                pieceType = PieceType::KNIGHT;
            }
            else if (mirrorCheck(j, 2))
            {
                pieceType = PieceType::BISHOP;
            }
            else if (j == 3)
            {
                pieceType = PieceType::QUEEN;
            }
            else if (j == 4)
            {
                pieceType = PieceType::KING;
            }
            else
            {
                throw std::logic_error("Invalid piece type");
            }

            this->_pieces[i][j] = Piece::fromType(pieceType, *this, Point(j, i), player);
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
