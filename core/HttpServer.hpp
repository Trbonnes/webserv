#if !defined(HTTPSERVER)
#define HTTPSERVER



#include "Config.hpp"
#include "HttpWorker.hpp"
#include "ProcessManager.hpp"
#include "ListenSocket.hpp"

#include <fstream>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <signal.h>


class WorkersInitException: public std::exception
{
	public:
		const char * what () const throw ()
    	{
    		return "Workers failed to initialize"; // TO DO reimplement into the cpp file
    	}
};


class HttpServer
{
public:
	HttpServer();
	~HttpServer();

private:
	Config						*_config;
	std::vector<ListenSocket>	_listen_sockset;
	pid_t*						_workers_pid; // might put into ProcessManager.hpp
	HttpServer(const HttpServer &);
	HttpServer &operator=(const HttpServer &);
	void			initConf();
	void			initListenSocket();
	void			initWorkers();
	void			masterLifecycle();
	static	void	launchProcess(); // TO DO is it clean ?

public:
	void			run();

};
#endif // HTTPSERVER

