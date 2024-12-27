#include <cstdlib>
#include <iostream>

#include "ConsoleChessboardApplication.h"
#include "CommonNamedPipe.h"


static void startBackend();
static void gameLifecycle(NAMED_PIPE& pipe);
static bool processMoveMsg(const std::string& msg, chess::Board& chessboard, NAMED_PIPE& pipe);
static bool processPawnChangingMsg(const std::string& msg, chess::Board& chessboard, NAMED_PIPE& pipe);
// static void clearConsole();

int main(int argc, char const *argv[])
{
    startBackend();
    return 0;
}


/**
 * Starts the backend.
 * Waits for the message "rdy", and then begins the game.
 */
static void startBackend()
{
    NAMED_PIPE pipe;
    std::cout << "Opening backend pipe..." << std::endl;
    // On Windows, this will block until a new connection arrives.
    // On Linux, it will just pass (mkfifo) and wait on waitForMessage.
    pipe.open();

    std::cout << "Backend connection opened. Waiting for frontend handshake..." << std::endl;
    const std::string msg = pipe.waitForMsg();

    if (msg != "rdy")
        throw std::runtime_error("Frontend connection failed: Unexpected message received");

    std::cout << "Connection established. Starting game...\n" << std::endl;
    gameLifecycle(pipe);
}

/**
 * Begins the game's lifecycle.
 * Sends "rdy" once ready.
 * 
 * The frontend should send movement strings appended by player number,
 * such as "e3d20".
 * 
 * When on a pawn-changing state, the frontend should send
 * a string in the format of piece type to player type,
 * such as "10" (King for White).
 * See `chess::PieceType` for all piece type codes.
 * The switching pawn should be agreed upon by both sides, and thus
 * does not need to be supplied necessarily.
 * 
 * The backend will respond with simply the appropriate status code.
 * Upon request error, the server will return "wtf".
 * 
 * Will exit once recieved "ext".
 */
static void gameLifecycle(NAMED_PIPE& pipe)
{
    chess::Board chessboard;
    ConsoleChessboardApplication consoleApp(chessboard);

    consoleApp.draw();
    pipe.sendMsg("rdy");

    while (true)
    {
        const std::string msg = pipe.waitForMsg();
        if (msg == "ext")
            break;

        bool processRes;

        if (chessboard.isInSwitchPawnState())
        {
            processRes = processPawnChangingMsg(msg, chessboard, pipe);
        }
        else
        {
            processRes = processMoveMsg(msg, chessboard, pipe);
        }

        if (!processRes)
        {
            pipe.sendMsg("wtf");
            continue;
        }

        consoleApp.draw();
    };

    std::cout << "\nQuitting..." << std::endl;
}

// Message processors.
// Will return false when the message failed to process.

static bool processMoveMsg(const std::string& msg, chess::Board& chessboard, NAMED_PIPE& pipe)
{
    if (msg.length() != 5)
        return false;

    const chess::Player& player = chessboard.getPlayer(msg[4] - '0');

    pipe.sendMsg(std::to_string(
        (int) chessboard.movePiece(msg.substr(0, 4), player))
    );
    return true;
}
static bool processPawnChangingMsg(const std::string& msg, chess::Board& chessboard, NAMED_PIPE& pipe)
{
    if (msg.length() != 2)
        return false;

    chess::Player& player = chessboard.getPlayer(msg[1] - '0');
    const chess::PieceType pieceType = (chess::PieceType) (msg[0] - '0');

    pipe.sendMsg(std::to_string(
        (int) chessboard.switchPawnTo(pieceType, player))
    );
    return true;
}


// static void clearConsole()
// {
//     // Using Linux for development rn;
//     // You probably won't be.
//     #if defined(_WIN32) || defined(_WIN64)
//         system("cls");
//     #else
//         system("clear");
//     #endif
// }
