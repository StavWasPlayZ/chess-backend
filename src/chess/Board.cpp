#include "Board.h"

using namespace chess;

#include "./piece/Pawn.h"
#include "./piece/King.h"
#include "./util/Utils.h"

#include <sstream>


const PieceType Board::_PAWN_SWITCHABLES[Board::_PAWN_SWITCHABLES_SIZE] =
{
    PieceType::QUEEN,
    PieceType::ROOK,
    PieceType::BISHOP,
    PieceType::KNIGHT
};


chess::Board::Board() :
    _playerTurn(0),
    _switchingPawn(nullptr)
{
    for (size_t i = 0; i < 2; i++)
    {
        this->_players[i] = new Player(*this, i);
    }

    _populateBoard();
}

chess::Board::~Board()
{
    _freeBoard();

    for (const Player* player : this->_players)
    {
        delete player;
    }
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

    // We also check this here to correctly return
    // the MoveResult::OUT_OF_BOUNDS error before
    // MoveResult::NO_TOOL throws.
    if (source.isOutOfBounds() || destination.isOutOfBounds())
        return MoveResult::OUT_OF_BOUNDS;

    Piece* const piece = getPieceAt(source);
    if (piece == nullptr)
        return MoveResult::NO_TOOL;

    Point orgPos = *piece->getPosition();

    Piece* const overPiece = getPieceAt(destination);

    // Can't eat le'king
    if ((overPiece != nullptr) && (overPiece->getType() == PieceType::KING))
        return MoveResult::ILLEGAL_MOVE;


    MoveResult res = piece->validateMove(destination);

    if (!util::isLegal(res))
        return res;


    _moveInternal(*piece, destination);

    if (overPiece != nullptr)
    {
        overPiece->getPlayer().removePiece(*overPiece);
    }

    // Check for self-check
    Player* const checkPlayer = getCheckPlayer();
    if ((checkPlayer != nullptr) && (initiator != *checkPlayer))
    {
        // Revert board
        _setPieceAt(source, *piece);

        if (overPiece == nullptr)
        {
            removePieceAt(destination);
        }
        else
        {
            _setPieceAt(destination, *overPiece);
            overPiece->getPlayer().addPiece(*overPiece);
        }

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

    piece->onMoved(res, orgPos, overPiece);
    // onMoved may have performed a special move; re-calculate the checkPlayer.
    _acknowledgeCheckResult(res, initiator);

    if (!isInSwitchPawnState())
    {
        this->_playerTurn = 1 - this->_playerTurn;
    }

    _notifyBoardUpdate(BoardOperationType::MOVE);
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

    // Replace the old piece with the new
    _setPieceAt(pos, *newPiece);

    initiator.removePiece(*this->_switchingPawn);
    initiator.addPiece(*newPiece);

    // Check for self-check
    Player* const checkPlayer = getCheckPlayer();
    if ((checkPlayer != nullptr) && (initiator != *checkPlayer))
    {
        // Revert piece
        _setPieceAt(pos, *this->_switchingPawn);

        initiator.removePiece(*newPiece);
        initiator.addPiece(*this->_switchingPawn);

        delete newPiece;

        return MoveResult::SELF_CHECK;
    }


    MoveResult res = MoveResult::LEGAL_MOVE;

    _acknowledgeCheckResult(res, initiator, checkPlayer);

    delete this->_switchingPawn;
    this->_switchingPawn = nullptr;

    this->_playerTurn = 1 - this->_playerTurn;

    _notifyBoardUpdate(BoardOperationType::PAWN_SWAP);
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
    for (Player* player : this->_players)
    {
        // The opposing king
        const King& otherKing = player->getOther().getKing();

        // Vs. this player's pieces
        for (const Piece* piece : player->getPieces())
        {
            const MoveResult moveToKingRes = piece->validateMove(*otherKing.getPosition());
            if (util::isLegal(moveToKingRes))
            {
                // Piece may move to king - AKA check.
                return player;
            }
        }
    }

    return nullptr;
}

Player *chess::Board::getCheckmatePlayer(Player& checkPlayer)
{
    // In a checkmate, the king is unable to move at all.
    King& otherKing = checkPlayer.getOther().getKing();
    const Point orgPos = *otherKing.getPosition();

    Player* result = &checkPlayer;

    // We're gonna be moving the king a bunch of times.
    removePieceAt(orgPos);

    // King may move to 8 places at most,
    // where the first is just 0, 0.
    for (int i = 1; i < 9; i++)
    {
        Point dest = Point(i / 2, i % 2);

        if (dest.x == 0 && dest.y == 0)
            continue;

        if (i > 3)
        {
            dest.x *= -1;
            dest.y *= -1;
        }

        dest += orgPos;
        Piece* destPiece = getPieceAt(dest);

        // Even king can't eat other le'king
        if ((destPiece != nullptr) && (destPiece->getType() == PieceType::KING))
            continue;

        if (!util::isLegal(otherKing.validateMove(dest)))
            continue;
        
        _setPieceAt(dest, otherKing);
        
        // If checkPlayer is still at check after the king "escaped",
        // it means this route is blocked.
        const Player* checkPlayerAfter = getCheckPlayer();
        if ((checkPlayerAfter == nullptr) || (*checkPlayerAfter != checkPlayer))
        {
            // King may escape; no checkmate.
            result = nullptr;
        }

        if (destPiece != nullptr)
        {
            _setPieceAt(dest, *destPiece);
        }
        else
        {
            removePieceAt(dest);
        }

        // A single route is all it takes.
        if (result == nullptr)
            break;
    }
    
    _setPieceAt(orgPos, otherKing);
    return result;
}

Piece* const (&chess::Board::getPieces() const)[Board::BOARD_SIZE][Board::BOARD_SIZE]
{
    return this->_pieces;
}

Piece *chess::Board::getPieceAt(const Point &point) const
{
    if (point.isOutOfBounds())
        return nullptr;
    
    return this->_pieces[point.y][point.x];
}

bool chess::Board::hasPieceAt(const Point &point) const
{
    return getPieceAt(point) != nullptr;
}

Player &chess::Board::getPlayer(const int index)
{
    return *this->_players[index];
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

void chess::Board::_acknowledgeCheckResult(MoveResult &currRes, const Player &player)
{
    return _acknowledgeCheckResult(currRes, player, getCheckPlayer());
}

void chess::Board::_acknowledgeCheckResult(MoveResult &currRes, const Player &player, Player *const checkPlayer)
{
    if (checkPlayer == nullptr)
        return;

    if (player != *checkPlayer)
        return;
    

    const Player* checkmatePlayer = getCheckmatePlayer(*checkPlayer);

    if ((checkmatePlayer != nullptr) && (player == *checkmatePlayer))
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

void chess::Board::_moveInternal(Piece &piece, const Point pos)
{
    removePieceAt(*piece.getPosition());
    _setPieceAt(pos, piece);
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

            Player& player = *this->_players[(i < (BOARD_SIZE / 2)) ? 1 : 0];

            PieceType pieceType;

            if (populationType == RowPopuplationType::PAWNS)
            {
                pieceType = PieceType::PAWN;
            }
            else
            {
                pieceType = _pieceTypeByColumn(j);
            }

            this->_pieces[i][j] = Piece::fromType(pieceType, *this, Point(j, i), player);
            this->_pieces[i][j]->onAddedToBoard();
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

void chess::Board::_notifyBoardUpdate(const BoardOperationType operationType)
{
    for (const Player* const player : this->_players)
    {
        for (Piece* const piece : player->getPieces())
        {
            piece->onBoardUpdated(operationType);
        }
    }
}

Board::RowPopuplationType chess::Board::_getRowPopulationType(const int row) const
{
    return
        util::mirrorCheck(row, 0)
            ? RowPopuplationType::ALL_ELSE
        : util::mirrorCheck(row, 1)
            ? RowPopuplationType::PAWNS
        : RowPopuplationType::NONE;
}

PieceType chess::Board::_pieceTypeByColumn(const int column) const
{
    if (util::mirrorCheck(column, 0))
    {
        return PieceType::ROOK;
    }
    else if (util::mirrorCheck(column, 1))
    {
        return PieceType::KNIGHT;
    }
    else if (util::mirrorCheck(column, 2))
    {
        return PieceType::BISHOP;
    }
    else if (column == 3)
    {
        return PieceType::QUEEN;
    }
    else if (column == 4)
    {
        return PieceType::KING;
    }
    else
    {
        throw std::logic_error("Invalid piece type");
    }
}
