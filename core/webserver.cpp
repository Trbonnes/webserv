#include "HttpServer.hpp"

int main(int argc, const char** argv) {

	HttpServer server;

	(void) argc;
	(void) argv;
	server = HttpServer();

	try
	{
		server.initConf();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		server.initListenSocket();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		server.initWorkers();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}