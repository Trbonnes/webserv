#include "ProcessManager.hpp"

void	sigint_handler(int sig)
{
	if (g_ismaster)
	{
		std::cerr << "Caught SIGINT signal, exiting...." << std::endl;
	}
	else
	{
		std::cerr << "Worker exiting" << std::endl;
	}
	exit(sig);
}