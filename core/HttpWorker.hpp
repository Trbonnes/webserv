#if !defined(HTTPWORKER)
#define HTTPWORKER

#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

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
	HttpWorker(std::vector<ListenSocket> &listen, Config* config);
	~HttpWorker();

private:
	HttpWorker(const HttpWorker &);
	HttpWorker &operator=(const HttpWorker &);
	std::vector<ListenSocket>	_listen_socket;
	Config						*_config;

public:
	void run();


};
#endif // HTTPWORKER