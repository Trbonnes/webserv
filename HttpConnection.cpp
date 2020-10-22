#include "HttpConnection.hpp"


void HttpConnection::write(char *data, size_t size) {
    send(_sock, data, size, 0);
}


int HttpConnection::getSock() {
	return _sock;
}

#include <iostream>

void HttpConnection::acceptOnSocket(int connection_sock) {

	socklen_t size;

	size = sizeof(_client_name);
	_sock = accept(connection_sock, &_client_name, &size);
	std::cout << "VALUE of RECEIPT" << _sock << std::endl;
	// TO DO throw error if accept fails
}
