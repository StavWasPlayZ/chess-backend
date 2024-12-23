#include "NamedPipeLinuxImpl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

NamedPipeLinuxImpl::NamedPipeLinuxImpl(const std::string &name) : INamedPipe(name.c_str()) {}
NamedPipeLinuxImpl::~NamedPipeLinuxImpl()
{
    close();
}

void NamedPipeLinuxImpl::sendMsg(const std::string &msg)
{
    int pFifo = openPipe(O_WRONLY);
    
    write(pFifo, msg.c_str(), msg.length());
    ::close(pFifo);
}

std::string NamedPipeLinuxImpl::waitForMsg()
{
    int pFifo = openPipe(O_RDONLY);

    char buffer[MAX_MSG_SIZE];
    ssize_t bytesRead = read(pFifo, buffer, sizeof(char) * (MAX_MSG_SIZE - 1));
    ::close(pFifo);

    buffer[bytesRead] = 0;
    return std::string(buffer);
}

void NamedPipeLinuxImpl::open()
{
    if (mkfifo(getName(), 0666) == -1)
        throw pipeNotCreatedException();
}
void NamedPipeLinuxImpl::close()
{
    unlink(getName());
}

int NamedPipeLinuxImpl::openPipe(const int flag) const
{
    int pFifo = ::open(getName(), flag);
    if (pFifo == -1)
        throw pipeNotOpenedException(strerror(errno));

    return pFifo;
}
