#include "HttpServer.hpp"


HttpServer::HttpServer() {
    _config = NULL;
    _manager = NULL;
	_config_path = "config/test.conf";
}

HttpServer &HttpServer::operator=(const HttpServer &s)
{
	_config = s._config;
	_manager = s._manager;
	_listen_sockset = s._listen_sockset;
	return *this;
}

HttpServer::~HttpServer() {
	if (_config)
		delete _config;
	if (_manager)
		delete _manager;
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
		// Log::debug("About to create workers");
		initWorkers();
<<<<<<< HEAD
		// Log::debug("Workers have been initated");
=======
		Log::debug("Workers have been instantiated");
>>>>>>> coreModule
	}
	catch(const std::exception& e)
	{
		// Log::debug("Is this real life ?");
		std::cerr << "Error while initializing workers : " << e.what() << '\n';
		std::cerr << e.what() << '\n';
		throw e;
	}
	masterLifecycle();
}

void HttpServer::initConf() {
	std::cout << "Initializing configuration" << std::endl;
	_config = NULL;
	try
	{
		int fd = open(_config_path.c_str(), O_RDONLY);
		if (fd == -1)
			throw OpenConfigfileFail();
		_config = configFileParser(fd);
		_manager = new ProcessManager();
		close(fd);
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
	std::vector<ConfigServer> servers;
	std::vector<int> ports;

	servers = _config->getServerList();
	// Initialize listening sockets that will be shared between workers
	for (std::vector<ConfigServer>::iterator it = servers.begin(); it < servers.end(); it++)
	{
		ports = it->getPort();
		for (size_t j = 0; j < ports.size(); j++)
			_listen_sockset.push_back(ListenSocket(ports[j])); // TO DO Check port already in use / Fix getPort()
	}
}

void			HttpServer::masterLifecycle()
{
	std::cout << "Master is entering is main lifecycle" << std::endl;
	_manager->manage();
	// TO DO add process management / reload mechanics
}

void HttpServer::initWorkers() {

	HttpWorker worker(_listen_sockset, _config);
	int nbworkers = _config->getWorker();

	if (nbworkers < WORKER_MIN || nbworkers > WORKER_MAX)
		nbworkers = WORKER_MIN;
    std::cout << "Initializing " << nbworkers << " workers" << std::endl;
	try
	{
		_manager->run(worker, nbworkers);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Cannot instantiate workers: " << e.what() << '\n';
		throw HttpServer::WorkersInitException();
	}
}

const char * HttpServer::WorkersInitException::what () const throw ()
{
    		return "Workers failed to initialize"; 
}

const char * HttpServer::OpenConfigfileFail	::what () const throw ()
{
    		return strerror(errno);
}

void			HttpServer::setConfigPath(std::string &path)
{
	_config_path = path;
}
