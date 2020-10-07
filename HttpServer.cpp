#include "HttpServer.hpp"


HttpServer::HttpServer() {
    
}

HttpServer::~HttpServer() {
    
}

void HttpServer::initConf() {
    std::cout << "Initializing configuration" << std::endl;
}

void HttpServer::initListenSocket() {
	std::cout << "Initializing listening sockets" << std::endl;

	int s = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)); // WItchcraft


	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( 3256 );
	int addrlen = sizeof(address);

	bind(s, (struct sockaddr *)&address,  
                                 sizeof(address));
	listen(s, 3);

	int newfd = accept(s, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen);
	printf("New connection !\n");

	char bf[] = "Server >> Hello, I am the server ! Who are you ?\nClient >> ";
	send(newfd, bf, std::strlen(bf), 0);

	char response[100];
	recv(newfd, response, 100, 0);

	char buff[100];
	sprintf(buff, "Server >> Goodbye %s !\n", response);
	send(newfd, buff, std::strlen(buff), 0);

	close(newfd);
}

void HttpServer::initWorkers() {
    std::cout << "Initializing workers" << std::endl;
}
