#include <cstdlib>
#include <iostream>

#include "ConsoleChessboardApplication.h"
#include "CommonNamedPipe.h"

static void clearConsole();

int main(int argc, char const *argv[])
{
    chess::Board chessboard;
    ConsoleChessboardApplication consoleApp(chessboard);

    //TODO: re-draw every time there is a change.
    // Also call clear.
    consoleApp.draw();

    return 0;
}

static void clearConsole()
{
    // Using Linux for development rn;
    // You probably won't be.
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}
