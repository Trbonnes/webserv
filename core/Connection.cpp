#include "Connection.hpp"

Connection::Connection(int listen_socket)
{
	socklen_t size;

	_request = NULL;
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


int Connection::getSock()
{
	return _socket;
}

void Connection::close()
{
	::close(_socket);
}

void Connection::setRequest(Socket *s)
{
	_request = s;
}


Socket *Connection::getRequest()
{
	return _request;
}

void Connection::clearRequest()
{
	delete _request;
	_request = NULL;
}

Connection::~Connection()
{
	if (_request)
		delete _request;
}

const char* Connection::Connectionfailed::what() const throw()
{
	return "Connection failed at creation";
}