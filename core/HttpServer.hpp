#if !defined(HTTPSERVER)
#define HTTPSERVER



#include "Config.hpp"
#include "HttpWorker.hpp"
#include "ProcessManager.hpp"
#include "ListenSocket.hpp"
#include "Log.hpp"

#include <fstream>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string>


#define WORKER_MAX 30 // TO DO is it un peu bourrin ?
#define WORKER_MIN 1

class HttpServer
{
public:
	HttpServer();
	~HttpServer();
	HttpServer &operator=(const HttpServer &);
	void			run();

	// Default configuration variables setters
	void setDefaultConfigPath(std::string &);
	class WorkersInitException: public std::exception
	{
	public:
		const char * what () const throw ();
	};

private:
	Config						*_config;
	std::vector<ListenSocket>	_listen_sockset;
	ProcessManager*				_manager;
	HttpServer(const HttpServer &);
	void			initConf();
	void			initListenSocket();
	void			initWorkers();
	void			masterLifecycle();

	// Default configuration variables
	std::string					_defaultconfigpath;
	

};
#endif // HTTPSERVER

