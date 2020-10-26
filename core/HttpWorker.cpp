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
	HttpConnection *new_connection; // temp pointer 
	HttpConnection* connections[FD_SETSIZE]; // array of connections
	ListenSocket* listening[FD_SETSIZE];

	std::cout << "Running a worker" << std::endl;

	// Transforming list in fdset and Listensocket hashmap (not sure about this name)
	FD_ZERO(&active_fs);
	for (unsigned int i = 0; i < _listen_socket.size(); i++)
	{
		listening[_listen_socket[i].getSock()] = &_listen_socket[i];
		FD_SET(_listen_socket[i].getSock(), &active_fs);
	}
	// Main loop of the worker
	while (1)
	{
		//read fs is going to be modified by select call, so we must reattribute the set there
		read_fs = active_fs;
		// Waiting for an event on listen socket
		if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1) // TO DO check if 0
		{
			std::cout << "Select error " << strerror(errno) << std::endl;
			continue; // TO DO throw something
		}
		std::cout << "Event occured" << std::endl;
		i = 0;
		for (i = 0; i < FD_SETSIZE; i++) // TO DO optimization ?
		{
			// if the fd is not set then there's no event on that fd, next
			if (!FD_ISSET(i, &read_fs))
				continue ;
			// if it is on a listening socket, create a new connection
			if (listening[i])
			{
				std::cout << "New connection" << std::endl; // TO DO remove or change log
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
			// If it is a connection socket, do the job
			else if (connections[i])
			{
				std::cout << "Event on connection" << std::endl;
				FD_CLR(i, &read_fs);
				// handle event with http Module
				connections[i]->read();
				connections[i]->write((char*)"You typed sum shiet\n", 20);
			}
			else
			{
				// TO DO or not ?
			}
		}
		// TO DO wero it just in case ?
		FD_ZERO(&read_fs);
	// TO DO timeout for http
	}
}
