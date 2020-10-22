#include "HttpWorker.hpp"


//TO DO The runnable arguments might depend from the configuration file
HttpWorker::HttpWorker(std::vector<ListenSocket> &listen) : Runnable(1, 1){
	std::cout << "Worker Initializing" << std::endl;
    _listen_socket = listen;
}

HttpWorker::~HttpWorker() {
}


// TO DO Check config var regarding max connections and max port/server block
void	HttpWorker::run()
{
	fd_set active_fs;
	fd_set read_fs;
	int i;
	HttpConnection *new_connection;
	HttpConnection* connections[FD_SETSIZE];
	ListenSocket* listening[FD_SETSIZE];

	std::cout << "Running a worker" << std::endl;

	// Transforming list in fdset and Listensocket hashmap (not sure about this name)
	FD_ZERO(&active_fs);
	for (unsigned int i = 0; i < _listen_socket.size(); i++)
	{
		listening[_listen_socket[i].getSock()] = &_listen_socket[i];
		FD_SET(_listen_socket[i].getSock(), &active_fs);
	}
	while (1)
	{
		read_fs = active_fs;
		if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1) // TO DO check if 0
		{
			std::cout << "Select error " << strerror(errno) << std::endl;
			
			continue; // TO DO throw something
		}
		std::cout << "Event occured" << std::endl;
		i = 0;
		for (i = 0; i < FD_SETSIZE; i++) // TO DO optimization ?
		{
			if (!FD_ISSET(i, &read_fs))
				continue ;
			if (listening[i])
			{
				std::cout << "New connection" << std::endl;
				try
				{
					new_connection = new HttpConnection();
					new_connection->acceptOnSocket(i);
					connections[new_connection->getSock()] = new_connection;
					FD_SET(new_connection->getSock(), &active_fs);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					delete new_connection;
				}
				
			}
			else if (connections[i])
			{
				std::cout << "Event on connection" << std::endl;
				// handle event with http Module
				connections[i]->write((char*)"Event on connection\n", 20);
			}
			else
			{
				// TO DO or not ?
			}
		}
		FD_ZERO(&read_fs);
	// TO DO timeout for http
	}
	std::cout << "Should not print" << std::endl;
}
