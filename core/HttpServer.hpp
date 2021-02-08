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


#define WORKER_MAX 128
#define WORKER_MIN 1

class ProcessManager;

class HttpServer
{
public:
	typedef enum status {
		RUNNING,
		STOPPING
	} e_status;

	HttpServer();
	~HttpServer();
	HttpServer &operator=(const HttpServer &);
	void			run();

	// Default configuration variables setters
	void setConfigPath(std::string &);
	e_status		getStatus();
	void			setStatus(e_status);
	void			killWorkers(int);
	class WorkersInitException: public std::exception
	{
	public:
		const char * what () const throw ();
	};
	class OpenConfigfileFail: public std::exception
	{
	public:
		const char * what () const throw ();
	};

private:
	Config						*_config;
	std::list<ListenSocket>	_listen_sockset;
	ProcessManager*				_manager;
	pthread_mutex_t				_accept_mutex;
	e_status					_status;
	HttpServer(const HttpServer &);
	void			initConf();
	void			initListenSocket();
	void			initWorkers();
	void			masterLifecycle();
	// Default configuration variables
	std::string					_config_path;
};

extern HttpServer	*g_server;
extern bool			g_ismaster;

#endif // HTTPSERVER

