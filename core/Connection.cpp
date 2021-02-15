#include "Connection.hpp"

Connection::Connection(int listen_socket, fd_set* r, fd_set* w) :
_active_read(r),
_active_write(w),
_module(*this)
{
	socklen_t size;

	size = sizeof(_client_name);
	_socket = ::accept(listen_socket, &_client_name, &size);
	if (_socket == -1)
		throw Connectionfailed();
	if (_socket >= FD_SETSIZE)
	{
		::close(_socket);
		throw Connectionfailed();
	}
}

Connection::Connection(const Connection& c) :
_module(const_cast<Connection&>(c))
{
	_socket = c._socket;
	_stream_read = c._stream_read;
	_stream_write = c._stream_write;
	_client_name = c._client_name;
	_active_read = c._active_read;
	_active_write = c._active_write;
}

Connection& Connection::operator=(const Connection& c)
{
	_socket = c._socket;
	_stream_read = c._stream_read;
	_stream_write = c._stream_write;
	_client_name = c._client_name;
	_active_read = c._active_read;
	_active_write = c._active_write;
	return (*this);
}

void Connection::subWrite(int fd)
{
	FD_SET(fd, _active_write);
}

void Connection::subRead(int fd)
{
	FD_SET(fd, _active_read);
}

void Connection::subStreamWrite(int fd)
{
	_stream_write = fd;
	FD_SET(fd, _active_write);
}

void Connection::subStreamRead(int fd)
{
	_stream_read = fd;
	FD_SET(fd, _active_read);
}

int	Connection::isWriteReady()
{
	return FD_ISSET(_socket, _active_write);
}

int	Connection::isReadReady()
{
	return FD_ISSET(_socket, _active_read);
}

int	Connection::isStreamWriteReady()
{
	return _stream_write != -1 && FD_ISSET(_stream_write, _active_write);
}

int	Connection::isStreamReadReady()
{
	return _stream_read != -1 && FD_ISSET(_stream_read, _active_read);
}

void Connection::write()
{
	try
	{
		BufferChain::writeBufferToFd(_write_chain, _socket);
	}
	catch(const IOError& e)
	{
		throw;
	}
}

void Connection::read()
{
	try
	{
		BufferChain::readToBuffer(_read_chain, _socket);
	}
	catch(const IOError& e)
	{
		throw;
	}
}

void Connection::streamWrite()
{}

void Connection::streamRead()
{}








BufferChain& Connection::getWriteChain()
{
	return _write_chain;
}

BufferChain& Connection::getReadChain()
{
	return _read_chain;
}

int Connection::getSock()
{
	return _socket;
}

void Connection::close()
{
	::close(_socket);
}

Connection::~Connection()
{
	close();
}

const char* Connection::Connectionfailed::what() const throw()
{
	return "Connection failed at creation";
}