#include "Connection.hpp"

Connection::Connection(int listen_socket)
{
	socklen_t size;

	_request = NULL;
	_response = NULL;
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

void Connection::setSocket(Socket *s)
{
	_request = s;
}


Socket *Connection::getSocket()
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