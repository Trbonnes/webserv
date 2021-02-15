#include "Connection.hpp"

Connection::Connection(int listen_socket)
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

Connection::Connection(const Connection& c)
{
	_socket = c._socket;
	_request = c._request;
	_client_name = c._client_name;
}

Connection& Connection::operator=(const Connection& c)
{
	_socket = c._socket;
	_request = c._request;
	_client_name = c._client_name;
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

int	Connection::write()
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

int	Connection::read()
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

int	Connection::streamWrite()
{}

int	Connection::streamRead()
{}












int Connection::getSock()
{
	return _socket;
}

void Connection::close()
{
	::close(_socket);
}

HttpRequest *Connection::getSocket()
{
	return _request;
}

void Connection::clearSocket()
{
	delete _request;
	_request = NULL;
}

HTTP* Connection::getMethod()
{
	return _response;
}

void Connection::setMethod(HTTP* r)
{
	_response = r;
}

Connection::~Connection()
{
	close();
	if (_request)
		delete _request;
}

const char* Connection::Connectionfailed::what() const throw()
{
	return "Connection failed at creation";
}