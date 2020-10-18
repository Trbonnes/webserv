#if !defined(HTTPSERVER)
#define HTTPSERVER

#include "HttpConf.hpp"
#include "HttpWorker.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>


class WorkersInitException: public std::exception
{
	public:
		const char * what () const throw ()
    	{
    		return "Workers failed to initialize";
    	}
};


class HttpServer
{
public:
	HttpServer();
	HttpServer(HttpServer &&) = default;
	HttpServer(const HttpServer &) = default;
	HttpServer &operator=(HttpServer &&) = default;
	HttpServer &operator=(const HttpServer &) = default;
	~HttpServer();

private:
	HttpConf	conf;
	fd_set		_sockset;
	pid_t*		_workers_pid;


public:
	void			initConf();
	void			initListenSocket();
	void			initWorkers();
	static	void	launchProcess();
};
#endif // HTTPSERVER

