#include "HttpConnection.hpp"

void HttpConnection::write(char *data, size_t size) {
	printf("data : %s", data);
    send(_sock, data, size, 0);
}


int HttpConnection::getSock() {
	return _sock;
}

#include <iostream>
#include <string.h>

HttpConnection::HttpConnection(ListenSocket& listen_sock) : _listen_sock(listen_sock)
{
}


void HttpConnection::accept() {

	socklen_t size;

	size = sizeof(_client_name);
	_sock = ::accept( _listen_sock.getSock(), &_client_name, &size);
	// TO DO throw error if accept fails
}


#include <string.h>
void HttpConnection::read() {
    
	char buff[1024]; // TO DO put buffer in class attribute	
	
	memset(buff, 0, 1024);
	std::cout << "MESSAGE RECIEVED" << std::endl;
	recv(_sock, buff, 1024, 0);
	std::cout << buff << std::endl;
}

int HttpConnection::getPort() {
	return _listen_sock.getPort();
}
