#pragma once
#include <string>

#define CHESS_PIPE_NAME "/tmp/cstavchess"
constexpr int MAX_MSG_SIZE = 5;

class NamedPipeLinuxImpl
{
public:
    NamedPipeLinuxImpl(const std::string& name = CHESS_PIPE_NAME);
    ~NamedPipeLinuxImpl();

    void sendMsg(const std::string& msg) const;
    std::string waitForMsg() const;

    const char* getName() const;

protected:
    /**
     * Open the pipe, and returns
     * its file descriptor.
     * Throws if something wrong occured.
     */
    int openPipe(const int flag) const;

private:
    const char* const _name;
};