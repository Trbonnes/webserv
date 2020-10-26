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

	_listen_sockset.push_back(ListenSocket());
	// Initialize listening sockets that will be shared between workers
}

//Define numver of workers

#include <signal.h>

void HttpServer::initWorkers() {

	int status;
	HttpWorker worker(_listen_sockset);


	int nbworkers = 3;

	_workers_pid = new pid_t[nbworkers]; // Change configuration

    std::cout << "Initializing workers" << std::endl;
	for (int i = 0; i < nbworkers; i++)
	{
		_workers_pid[i] = ProcessManager::launchProcess(worker);
	}
	for (int i = 0; i < nbworkers; i++)
	{
		wait(&status);
	}
}
