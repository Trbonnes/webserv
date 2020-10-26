#include "ListenSocket.hpp"


		#include <string.h>
ListenSocket::ListenSocket(int port) {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0)
	{
		// TO DO throw error
	}
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)); // TO DO WItchcraft
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port ); // program from configuration
	int addrlen = sizeof(address);
	(void) addrlen;

	if (bind(_sock, (struct sockaddr *)&address,
                                 sizeof(address)))
	{
		std::cout << strerror(errno) << std::endl;
		exit(0);
	}
	listen(_sock, 3);

	// TO DO error management
}

ListenSocket::~ListenSocket() {
    
}

int ListenSocket::getSock() {
    return _sock;
}
