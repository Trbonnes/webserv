#include "HttpServer.hpp"


HttpServer::HttpServer() {
    
}

HttpServer::~HttpServer() {
    
}

void HttpServer::initConf() {
	std::cout << "Initializing configuration" << std::endl;

	int fd = open("/home/user42/Bureau/webserver/config/test.conf", O_RDWR); // for test purposes

	try {
		_config = configFileParser(fd);

		std::cout << std::endl;

		std::cout << "-> Workers: " << _config->getWorker() << std::endl;
		std::cout << "-> Worker connections: " << _config->getWorkerConnections() << std::endl;

		// for (std::vector<ConfigServer>::iterator it = config->getServer().begin(); it != config->getServer().end(); it++) {
		// 	it->printServer();
		// }
		
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		//TO DO throw a custom error
	}

}

void HttpServer::initListenSocket() // TO DO optimization
{
	std::cout << "Initializing listening sockets" << std::endl;
	std::vector<ConfigServer> servers;
	std::map<std::string, Location, Compare<std::string>> locations;
	std::map<std::string, Location, Compare<std::string>>::iterator itl;
	servers = _config->getServer();
	// Initialize listening sockets that will be shared between workers
	for (size_t i = 0; i < servers.size(); i++)
	{
		_listen_sockset.push_back(ListenSocket(servers[i].getPort())); // TO DO Check port already in use
	}
}

//Define numver of workers

#include <signal.h>

void HttpServer::initWorkers() {

	int status;
	int nbworkers;
	HttpWorker worker(_listen_sockset);
	
	nbworkers = _config->getWorker();
	_workers_pid = new pid_t[nbworkers];
    std::cout << "Initializing workers" << std::endl;
	for (int i = 0; i < nbworkers; i++)
	{
		_workers_pid[i] = ProcessManager::launchProcess(worker); //TO DO rework the process manager to hold the process pid in the class
	}
	for (int i = 0; i < nbworkers; i++)
	{
		wait(&status);
	}
}
