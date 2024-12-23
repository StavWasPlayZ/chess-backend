#include "INamedPipe.h"

INamedPipe::INamedPipe(const std::string& name) : _name(name.c_str())
{
	// Make the callee open by themselves.
	//open();
}

const char* INamedPipe::getName() const
{
	return this->_name;
}

std::ios_base::failure INamedPipe::pipeNotCreatedException() const
{
	return std::ios_base::failure("Failed to create named pipe");
}

std::ios_base::failure INamedPipe::pipeNotOpenedException(const std::string& details) const
{
	return std::ios_base::failure("Failed to open named pipe " + std::string(getName()) + ": " + details);
}