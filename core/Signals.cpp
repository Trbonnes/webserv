#include "ProcessManager.hpp"

void	sig_handler(int sig)
{
	g_server->setStatus(HttpServer::STOPPING);
	if (g_ismaster)
	{
		std::cerr << "\nWebserv is shutting down...\n";
		g_server->killWorkers(sig);
	}
}