#include "HttpConnection.hpp"

HttpConnection::HttpConnection(ListenSocket& listen_sock) : _listen_sock(listen_sock)
{
}

HttpConnection::~HttpConnection()
{
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
	// TO DO throw error if accept fails
}


void HttpConnection::read() {
    
	char buff[1024]; // TO DO put buffer in class attribute	
	
	memset(buff, 0, 1024);
	recv(_sock, buff, 1024, 0);
	std::cout << buff << std::endl;
}

int HttpConnection::getPort() {
	return _listen_sock.getPort();
}
