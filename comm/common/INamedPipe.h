#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

constexpr int MAX_MSG_SIZE = 6;

class INamedPipe
{
public:
    INamedPipe(const std::string& name);

    virtual void sendMsg(const std::string& msg) = 0;
    virtual std::string waitForMsg() = 0;

    const char* getName() const;

    virtual void open() = 0;
    virtual void close() = 0;

protected:
    std::ios_base::failure pipeNotCreatedException() const;
    std::ios_base::failure pipeNotOpenedException(const std::string& details) const;

private:
    const char* const _name;
};