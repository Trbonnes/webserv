#include "HttpServer.hpp"


HttpServer::HttpServer() {
    _config = NULL;
    _manager = NULL;
	_config_path = "config/webserv.conf";
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
		initWorkers();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error while initializing workers : " << e.what() << '\n';
		std::cerr << e.what() << '\n';
		throw e;
	}
	masterLifecycle();
}

void HttpServer::initConf() {
	_config = NULL;
	try
	{
		int fd = open(_config_path.c_str(), O_RDONLY);
		if (fd == -1)
			throw OpenConfigfileFail();
		_config = configFileParser(fd);
		_manager = new ProcessManager();
		std::cerr << "Running configuration specified in : " << _config_path.c_str() << std::endl;
		close(fd);
	}
	catch (OpenConfigfileFail const &e) {
		if (_config)
			delete _config;
		throw e;
	}
}

void HttpServer::initListenSocket()
{
	// std::cerr << "Initializing listening sockets" << std::endl;
	std::vector<ConfigServer> servers;
	std::vector<int> ports;

	servers = _config->getServerList();
	// Initialize listening sockets that will be shared between workers
	for (std::vector<ConfigServer>::iterator it = servers.begin(); it < servers.end(); it++)
	{
		ports = it->getPort();
		for (size_t j = 0; j < ports.size(); j++)
			try
			{
				_listen_sockset.push_back(ListenSocket(ports[j]));
			}
			catch(const ListenSocket::ListenSocketException& e)
			{
				throw e;
			}
	}
}

void			HttpServer::masterLifecycle()
{
	_manager->manage();
}

void HttpServer::initWorkers() {
	int nbworkers = _config->getWorker();

	HttpWorker worker(_listen_sockset, _config);
	if (nbworkers < WORKER_MIN || nbworkers > WORKER_MAX)
		nbworkers = WORKER_MIN;
    // std::cerr << "Initializing " << nbworkers << " workers" << std::endl;
	try
	{
		_manager->run(worker, nbworkers, true);
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

void		HttpServer::setConfigPath(std::string &path)
{
	_config_path = path;
}
