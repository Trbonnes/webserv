#if !defined(HTTPSERVER)
#define HTTPSERVER

#include "HttpConf.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
<<<<<<< HEAD
#include <unistd.h>
#include <cstring>
=======
>>>>>>> 3163f9f0b3bcdba39cfc1e1696900474686944e6

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
	HttpConf conf;
	

public:
	void initConf();
	void initListenSocket();
	void initWorkers();
};
#endif // HTTPSERVER

