#if !defined(HTTPWORKER)
#define HTTPWORKER

#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "ListenSocket.hpp"
#include "Runnable.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "Connection.hpp"
#include "HttpServer.hpp"

class HttpWorker : public Runnable
{
public:
	HttpWorker(std::list<ListenSocket> &listen, Config* config);
	HttpWorker(const HttpWorker &);
	HttpWorker &operator=(const HttpWorker &);
	~HttpWorker();
	void run();
	Runnable* clone() const;

private:
	std::list<ListenSocket>	_listen_socket;
	std::list<Connection*>	_connections;
	Config						*_config;
	fd_set 					_active_read;
	fd_set 					_active_write;

	void	acceptConnection(int sock);
	void	closeConnections();
};
#endif // HTTPWORKER