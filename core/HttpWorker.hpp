#if !defined(HTTPWORKER)
#define HTTPWORKER

#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "ListenSocket.hpp"
#include "HttpConnection.hpp"
#include "Runnable.hpp"
#include "Config.hpp"

#include "../HTTP/HTTP.hpp"
#include "../HTTP/Socket.hpp"
#include "../HTTP/utils/utils.hpp"

class HttpWorker : public Runnable
{
public:
	HttpWorker(std::vector<ListenSocket> &listen, Config* config, pthread_mutex_t *accept_mutex);
	HttpWorker(const HttpWorker &);
	HttpWorker &operator=(const HttpWorker &);
	~HttpWorker();
	void run();
	Runnable* clone() const;

private:
	std::vector<ListenSocket>	_listen_socket;
	Config						*_config;
	pthread_mutex_t				*_accept_mutex;
};
#endif // HTTPWORKER