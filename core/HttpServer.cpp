#include "HttpServer.hpp"


HttpServer::HttpServer() {
    _config = NULL;
    _workers_pid = NULL;
}

HttpServer::~HttpServer() {
	if (_config)
		delete _config;
	if (_workers_pid)
		delete _workers_pid;
}


void HttpServer::run()
{
	try
	{
		initConf();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error while initializing configuration : " << e.what() << '\n';
		throw e;
	}
	try
	{
		initListenSocket();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error while initializing listen socket : " << e.what() << '\n';
		throw e;
	}
	try
	{
		// initWorkers();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error while initializing workers : " << e.what() << '\n';
		std::cerr << e.what() << '\n';
		throw e;
	}
}

void HttpServer::initConf() {
	std::cout << "Initializing configuration" << std::endl;

	int fd = open("test/test.conf", O_RDWR); // for test purposes

	_config = 0;
	try {
		_config = configFileParser(fd);
		std::cout << "Workers: " << _config->getWorker() << std::endl;
		std::cout << "Worker connections: " << _config->getWorkerConnections() << std::endl;

	}
	catch (std::exception const &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		if (_config)
			delete _config;
		throw e;
	}
}

void HttpServer::initListenSocket() // TO DO optimization
{
	std::cout << "Initializing listening sockets" << std::endl;
	std::map<std::string, Location, Compare<std::string>>::iterator itl;
	std::map<std::string, Location, Compare<std::string>> locations;
	std::vector<ConfigServer> servers;
	std::vector<int> ports;

	servers = _config->getServerList();
	// Initialize listening sockets that will be shared between workers
	for (size_t i = 0; i < servers.size(); i++)
	{
		ports = servers[i].getPort();
		for (size_t j = 0; j < ports.size(); j++)
		{
			_listen_sockset.push_back(ListenSocket(ports[j])); // TO DO Check port already in use / Fix getPort()
		}
	}
}

//Define numver of workers

#include <signal.h>

void HttpServer::initWorkers() {

	int status;
	int nbworkers;
	HttpWorker worker(_listen_sockset, _config);
	
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
