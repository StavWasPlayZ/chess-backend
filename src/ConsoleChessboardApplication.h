#include "./chess/Board.h"

class ConsoleChessboardApplication
{
public:
    ConsoleChessboardApplication(chess::Board& board);

    void draw() const;

    chess::Board& getBoard() const;

private:
    chess::Board& _board;

    std::string prettifiedBoard() const;
};