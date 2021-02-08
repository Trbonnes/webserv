#include "ProcessManager.hpp"

void	sig_handler(int sig)
{
	g_server->setStatus(HttpServer::STOPPING);
	if (g_ismaster)
	{
		std::cerr << "Caught SIGINT signal, exiting....\n";
		g_server->killWorkers(sig);
	}
	else
	{
		std::cerr << "Worker exiting\n";
	}
	(void) sig;
	// exit(sig);
}