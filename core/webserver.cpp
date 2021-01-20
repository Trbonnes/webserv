#include "HttpServer.hpp"
#include <string>


static void printUsage()
{
	std::cerr << "Usage: ./webserv [option argument]" << std::endl;
	std::cerr << std::endl;
	std::cerr << "Available options:" << std::endl;
	std::cerr << "\r-c : set the path to configuration file to the next argument" << std::endl;
}

static int parseArguments(HttpServer &server, const char **av)
{
	size_t i;
	std::string opt;
	std::string arg;


	i = 1;
	while (av[i])
	{
		if (av[i][0] == '-')
		{
			if (av[i + 1])
			{
				opt = std::string(av[i]);
				arg = std::string(av[i+1]);
				if (opt == "-c")
					server.setConfigPath(arg);
				else
					return -1;
				i++;
			}
			else
				return -1;
		}
		else
			return -1;
		i++;
	}
	return (0);
}


int main(int ac, const char** av, const char** env) {

	HttpServer server;

	(void) ac;
	(void) av;
	(void) env;

	if (ac > 1)
	{
		if (parseArguments(server, av))
		{
			printUsage();
			return 1;
		}
	}
	try
	{
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Erro while running server :" << e.what() << '\n';
		return 1;
	}
	return 0;
}