#pragma once
#include <string>

#include "../common/INamedPipe.h"

constexpr const char* CHESS_PIPE_NAME = "/tmp/cstavchess";

class NamedPipeLinuxImpl : public INamedPipe
{
public:
    NamedPipeLinuxImpl(const std::string& name = CHESS_PIPE_NAME);

    virtual void sendMsg(const std::string& msg) override;
    virtual std::string waitForMsg() override;

    virtual void open() override;
    virtual void close() override;

protected:
    /**
     * Open the pipe, and returns
     * its file descriptor.
     * Throws if something wrong occured.
     */
    int openPipe(const int flag) const;
};