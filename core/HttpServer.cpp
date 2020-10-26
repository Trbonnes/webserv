#include "HttpServer.hpp"


HttpServer::HttpServer() {
    
}

HttpServer::~HttpServer() {
    
}

void HttpServer::initConf() {
	std::cout << "Initializing configuration" << std::endl;

	int fd = open("/home/user42/Bureau/webserver/config/test.conf", O_RDWR); // for test purposes

	try {
		Config *config = configFileParser(fd);

		std::cout << std::endl;

		std::cout << "-> Workers: " << config->getWorker() << std::endl;
		std::cout << "-> Worker connections: " << config->getWorkerConnections() << std::endl;

		// for (std::vector<ConfigServer>::iterator it = config->getServer().begin(); it != config->getServer().end(); it++) {
		// 	it->printServer();
		// }
		
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		//TO DO throw a custom error
	}

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
