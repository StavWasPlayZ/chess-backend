#include "NamedPipeWindowsImpl.h"

NamedPipeWindowsImpl::NamedPipeWindowsImpl(const std::string& name) :
	INamedPipe(name),
	_pipe(nullptr)
{}
NamedPipeWindowsImpl::~NamedPipeWindowsImpl()
{
	close();
}


void NamedPipeWindowsImpl::sendMsg(const std::string& msg)
{
	DWORD written;
	WriteFile(*this->_pipe, msg.c_str(), msg.length() + 1, &written, nullptr);
}

std::string NamedPipeWindowsImpl::waitForMsg()
{
	char buffer[MAX_MSG_SIZE];

	DWORD read;
	ReadFile(*this->_pipe, buffer, MAX_MSG_SIZE, &read - 1, nullptr);
	buffer[read] = 0;

	return std::string(buffer);
}

void NamedPipeWindowsImpl::open()
{
	delete _pipe;
	this->_pipe = new HANDLE;

	*this->_pipe = CreateNamedPipe(
		getName(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		MAX_MSG_SIZE,
		MAX_MSG_SIZE,
		0,
		nullptr
	);

	if (*_pipe == INVALID_HANDLE_VALUE)
	{
		delete _pipe;
		throw pipeNotOpenedException("" + GetLastError());
	}

	waitForConnection();
}

void NamedPipeWindowsImpl::close()
{
	if (_pipe == nullptr)
		return;

	DisconnectNamedPipe(*this->_pipe);
	CloseHandle(*this->_pipe);
	delete _pipe;
	this->_pipe = nullptr;
}

void NamedPipeWindowsImpl::waitForConnection() const
{
	if (!ConnectNamedPipe(*this->_pipe, nullptr))
		throw pipeNotOpenedException("" + GetLastError());
}


