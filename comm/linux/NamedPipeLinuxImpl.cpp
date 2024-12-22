#include "NamedPipeLinuxImpl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

NamedPipeLinuxImpl::NamedPipeLinuxImpl(const std::string &name) : _name(name.c_str())
{
    if (mkfifo(getName(), 0666) == -1)
        throw std::ios_base::failure("Failed to create named pipe");
}

NamedPipeLinuxImpl::~NamedPipeLinuxImpl()
{
    unlink(getName());
}

void NamedPipeLinuxImpl::sendMsg(const std::string &msg) const
{
    int pFifo = openPipe(O_WRONLY);
    
    write(pFifo, msg.c_str(), msg.length());
    close(pFifo);
}

std::string NamedPipeLinuxImpl::waitForMsg() const
{
    int pFifo = openPipe(O_RDONLY);

    char buffer[MAX_MSG_SIZE];
    ssize_t bytesRead = read(pFifo, buffer, sizeof(char) * (MAX_MSG_SIZE - 1));
    close(pFifo);

    buffer[bytesRead] = 0;
    return std::string(buffer);
}

const char *NamedPipeLinuxImpl::getName() const
{
    return this->_name;
}

int NamedPipeLinuxImpl::openPipe(const int flag) const
{
    int pFifo = open(getName(), flag);
    if (pFifo == -1)
        throw std::ios_base::failure("Failed to open named pipe " + std::string(getName()) + ": " + strerror(errno));

    return pFifo;
}
