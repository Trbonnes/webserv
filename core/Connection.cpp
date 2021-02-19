#include "Connection.hpp"

Connection::Connection(int listen_socket, fd_set* r, fd_set* w, Config* config) :
_active_read(r),
_active_write(w),
_module(*this)
{
	socklen_t size;

	_stream_write = -1;
	_stream_read = -1;
	_module.setConfig(config);
	size = sizeof(_client_name);
	_socket = ::accept(listen_socket, &_client_name, &size);
	if (_socket == -1)
		throw Connectionfailed();
	if (_socket >= FD_SETSIZE)
	{
		::close(_socket);
		throw Connectionfailed();
	}
	subRead();
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

void Connection::subWrite()
{
	FD_SET(_socket, _active_write);
}

void Connection::subRead()
{
	FD_SET(_socket, _active_read);
}

void Connection::setStreamWrite(int fd)
{
	_stream_write = fd;
}

void Connection::subStreamWrite()
{
	FD_SET(_stream_write, _active_write);
}

void Connection::setStreamRead(int fd)
{
	_stream_read = fd;
}

void Connection::subStreamRead()
{
	FD_SET(_stream_read, _active_read);
}

void Connection::unsubWrite()
{
	FD_CLR(_socket, _active_write);
}

void Connection::unsubRead()
{
	FD_CLR(_socket, _active_read);
}

void Connection::unsubStreamWrite()
{
	FD_CLR(_stream_write, _active_write);
}

void Connection::unsubStreamRead()
{
	FD_CLR(_stream_read, _active_read);
}

int	Connection::isWriteReady(fd_set* set)
{
	return FD_ISSET(_socket, set);
}

int	Connection::isReadReady(fd_set* set)
{
	return FD_ISSET(_socket, set);
}

int	Connection::isStreamWriteReady(fd_set* set)
{
	return _stream_write != -1 && FD_ISSET(_stream_write, set);
}

int	Connection::isStreamReadReady(fd_set* set)
{
	return _stream_read != -1 && FD_ISSET(_stream_read, set);
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
	int read;
	
	try
	{
		read = BufferChain::readToBuffer(_read_chain, _socket);
	}
	catch(const IOError& e)
	{
		throw;
	}
	if (read == 0)
	{
	 	throw ConnectionClose();
	}
	_module.handleRead();
}

void Connection::streamWrite()
{
	try
	{
		_module.handleStreamWrite();
	}
	catch(const IOError& e)
	{
		throw;
	}
}

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
	if (_socket != -1)
		::close(_socket);
	if (_stream_read != -1)
		::close(_stream_read);
	if (_stream_write != -1)
		::close(_stream_read);
}

Connection::~Connection()
{
	//unsubbing fd from sets
	unsubRead();
	unsubWrite();
	unsubStreamRead();
	unsubStreamWrite();
	//closing all the fds
	close();
}

const char* Connection::Connectionfailed::what() const throw()
{
	return "Connection failed at creation";
}

const char * Connection::ConnectionClose::what() const throw ()
{
	return "Connection close";
}
