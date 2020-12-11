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


#define WORKER_MAX 30
#define WORKER_MIN 1

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
	HttpServer &operator=(const HttpServer &);

private:
	Config						*_config;
	std::vector<ListenSocket>	_listen_sockset;
	ProcessManager*				_manager;
	HttpServer(const HttpServer &);
	void			initConf();
	void			initListenSocket();
	void			initWorkers();
	void			masterLifecycle();
	static	void	launchProcess(); // TO DO is it clean ?

public:
	void			run();

};
#endif // HTTPSERVER

