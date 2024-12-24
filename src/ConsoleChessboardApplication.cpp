#include "ConsoleChessboardApplication.h"

#include <sstream>
#include <iostream>

ConsoleChessboardApplication::ConsoleChessboardApplication(chess::Board &board) :
    _board(board)
{}

void ConsoleChessboardApplication::draw() const
{
    std::cout
        << prettifiedBoard() << std::endl
        << std::endl
        << "Player 1 points: " << getBoard().getPlayer(0).getPointsTotal() << std::endl
        << "Player 2 points: " << getBoard().getPlayer(1).getPointsTotal() << std::endl;
}

chess::Board &ConsoleChessboardApplication::getBoard() const
{
    return this->_board;
}

std::string ConsoleChessboardApplication::prettifiedBoard() const
{
    const std::string boardRaw = getBoard().asPiecesString();
    std::ostringstream builder;

    for (size_t i = 0; i < chess::Board::BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < chess::Board::BOARD_SIZE; j++)
        {
            builder << boardRaw[i * chess::Board::BOARD_SIZE + j];

            if (j != (chess::Board::BOARD_SIZE - 1))
            {
                builder << ' ';
            }
        }

        if (i != (chess::Board::BOARD_SIZE - 1))
        {
            builder << '\n';
        }
    }
    
    return builder.str();
}
