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
	fd_set listen_fs;
	fd_set active_fs;
	fd_set read_fs;
	int listensize;
	int i,j;
	HttpConnection *new_connection;
	HttpConnection* connections[FD_SETSIZE];
	ListenSocket* listening[FD_SETSIZE];

	std::cout << "Running a worker" << std::endl;

	// Transforming list in fdset and Listensocket hashmap (not sure about this name)
	listensize = _listen_socket.size();
	for (int i = 0; i < listensize; i++)
	{
		listening[_listen_socket[i].getSock()] = &_listen_socket[i];
		FD_SET(_listen_socket[i].getSock(), &listen_fs);
	}
	active_fs = listen_fs;
	while (1)
	{
		read_fs = active_fs;
		if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1) // TO DO check if 0
		{
			continue; // TO DO throw something
		}
		i = 0;
		for (i = 0; i < FD_SETSIZE; i++) // TO DO optimization ?
		{
			if (!FD_ISSET(i, &read_fs))
				continue ;
			if (listening[i])
			{
				try
				{
					new_connection = new HttpConnection();
					new_connection->accept(i);
					connections[new_connection->getSock()] = new_connection;
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					delete new_connection;
				}
				
			}
			if (connections[i])
			{

			}
			else
			{
				// TO DO or not ?
			}
		}
	// TO DO timeout for http
	}
}


bool HttpWorker::is_on_listen_sockset()
{
	return false;
}
