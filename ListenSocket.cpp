#include "ListenSocket.hpp"


ListenSocket::ListenSocket() {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)); // WItchcraft
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 3256 ); // program from configuration
	int addrlen = sizeof(address);
	(void) addrlen;

	bind(_sock, (struct sockaddr *)&address,
                                 sizeof(address));
	listen(_sock, 3);

	// TO DO error management
}

ListenSocket::~ListenSocket() {
    
}

int ListenSocket::getSock() {
    return _sock;
}
