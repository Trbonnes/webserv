#include "ProcessManager.hpp"

void	sig_handler(int sig)
{
	g_singaled++;
	g_server->setStatus(HttpServer::STOPPING);
	if (g_ismaster)
	{
		if (g_singaled > 1)
		{
			std::cerr << "Force killing workers\n";
			g_server->killWorkers(SIGKILL);
		}
		else
		{
			std::cerr << "\nWebserv is shutting down gently, resend the signal to force kill workers\n";
			g_server->killWorkers(sig);
		}
	}
}

void	sigpipe_handler(int sig)
{
	std::cout << "Broken Pipe" << std::endl;
	(void) sig;
}