#include "HttpConnection.hpp"

HttpConnection::HttpConnection(ListenSocket& listen_sock) : _listen_sock(listen_sock)
{
}

HttpConnection::~HttpConnection()
{
	close(_sock);
}

void HttpConnection::write(char *data, size_t size) {
    send(_sock, data, size, 0);
}

int HttpConnection::getSock() {
	return _sock;
}

void HttpConnection::accept() {

	socklen_t size;

	size = sizeof(_client_name);
	_sock = ::accept( _listen_sock.getSock(), &_client_name, &size);
	// setsockopt(_sock, )
	if (_sock == -1)
		throw HttpConnection::AcceptFailed();
	
}

int HttpConnection::getPort() {
	return _listen_sock.getPort();
}

const char * HttpConnection::ConnectionClose::what () const throw ()
{
	return "Connection has been closed";
}

const char * HttpConnection::AcceptFailed::what () const throw ()
{
	return strerror(errno); // Maybe add Port or something
}