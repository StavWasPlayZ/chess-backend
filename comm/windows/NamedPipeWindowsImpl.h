#pragma once

#include "../common/INamedPipe.h"

#include <string>
#include <windows.h>

constexpr const char* CHESS_PIPE_NAME = R"(\\.\pipe\cstavchess)";

class NamedPipeWindowsImpl : public INamedPipe
{
public:
    NamedPipeWindowsImpl(const std::string& name = CHESS_PIPE_NAME);
    ~NamedPipeWindowsImpl();

    void sendMsg(const std::string& msg) override;
    std::string waitForMsg() override;

    virtual void open() override;
    virtual void close() override;

protected:
    HANDLE* _pipe;

    /**
    * Wait until a connection to the pipe is established.
    */
    void waitForConnection() const;
};