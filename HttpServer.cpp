#include "HttpServer.hpp"


HttpServer::HttpServer() {
    
}

HttpServer::~HttpServer() {
    
}

void HttpServer::initConf() {
    std::cout << "Initializing configuration" << std::endl;

	// Read configuration and put it in a structure/class

}

void HttpServer::initListenSocket() {
	std::cout << "Initializing listening sockets" << std::endl;


	// Initialize listening sockets that will be shared between workers
	FD_ZERO(&_listen_sockset);
	int s = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)); // WItchcraft
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 3256 );
	int addrlen = sizeof(address);
	(void) addrlen;

	bind(s, (struct sockaddr *)&address,
                                 sizeof(address));
	listen(s, 3);
	FD_SET(s, &_listen_sockset);
}

//Define numver of workers

#include <signal.h>

void HttpServer::initWorkers() {

	int status;
	HttpWorker worker;

	//init worker

	_workers_pid = new pid_t[2];
    std::cout << "Initializing workers" << std::endl;
	for (size_t i = 0; i < 2; i++)
	{
		_workers_pid[i] = ProcessManager::launchProcess(worker);
	}
	for (size_t i = 0; i < 2; i++)
	{
		wait(&status);
	}
}
