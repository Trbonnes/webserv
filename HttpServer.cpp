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

	_listen_sockset.push_front(ListenSocket());
	// Initialize listening sockets that will be shared between workers
}

//Define numver of workers

#include <signal.h>

void HttpServer::initWorkers() {

	int status;
	HttpWorker worker(_listen_sockset);

	_workers_pid = new pid_t[2]; // Change configuration

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
