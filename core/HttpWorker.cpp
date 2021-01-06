#include "HttpWorker.hpp"


//TO DO The runnable arguments might depend from the configuration file
HttpWorker::HttpWorker(std::vector<ListenSocket> &listen, Config* config, pthread_mutex_t *accept_mutex) : Runnable(1, 1)
{
	std::cout << "Worker Initializing" << std::endl;
	_config = config;
    _listen_socket = listen;
	_accept_mutex = accept_mutex;
}

HttpWorker::HttpWorker(const HttpWorker &w) : Runnable(w)
{
	std::cout << "Worker Initializing" << std::endl;
	_config = w._config;
	_accept_mutex = w._accept_mutex;
    _listen_socket = w._listen_socket;
}

HttpWorker::~HttpWorker() {
}


// TO DO Check config var regarding max connections and max port/server block
void	HttpWorker::run()
{
	fd_set 	active_fs;
	fd_set 	read_fs;
	int 	i;
	char* 	response;
	int		responseSize;
	HttpConnection*	new_connection; // temp pointer 
	HttpConnection*	connections[FD_SETSIZE]; // array of connections
	ListenSocket* 	listening[FD_SETSIZE]; // array of pointers

	std::cout << "Running a worker" << std::endl;
	// Important zeroing-out of the arrays
	ft_bzero(connections, FD_SETSIZE * sizeof(ListenSocket*)); 
	ft_bzero(listening, FD_SETSIZE * sizeof(ListenSocket*));
	// Transforming list in fdset and ListenSocket in an array we can access with i directly without looping through the fdset
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
		if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1)
		{
			std::cout << "Select error " << strerror(errno) << std::endl;
			continue; // TO DO throw something ?
		}
		std::cout << "Event occured"<< std::endl;
		i = 0;
		for (i = 0; i < FD_SETSIZE; i++)
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
					pthread_mutex_lock(_accept_mutex);
					new_connection = new HttpConnection(*listening[i]);
					new_connection->accept();
					connections[new_connection->getSock()] = new_connection;
					FD_SET(new_connection->getSock(), &active_fs);
					pthread_mutex_unlock(_accept_mutex);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					pthread_mutex_unlock(_accept_mutex);
					delete new_connection;
				}
				
			}
			// If it is a connection socket, do the job
			else if (connections[i])
			{
				std::cout << "Event on connection" << std::endl;
				FD_CLR(i, &read_fs);
				try
				{
					Socket *socket = httpRequestParser(connections[i]->getSock()); // TO DO why would it return a socket class and not an httpRequest object ? 
					ConfigServer &ptr2 = _config->getServerList()[0];
					HTTP method(socket, ptr2);
					std::cout << "ABOUT TO CREATE RESPONSE" << std::endl;
					response = method.getResponse(); // TO DO make code more modulare and clean up names
					responseSize = method.getResponseSize();
					std::cout << "RESPONSE CREATED" << std::endl << std::endl;
					std::cerr << response << std::endl;
					connections[i]->write(response, responseSize); // TO DO ugly
					std::cout << std::endl << "ENDING REQUEST" << std::endl;
				}
				catch(const HttpConnection::ConnectionClose& e)
				{
					int index = connections[i]->getSock();
					std::cerr << "Connection has been closed: " << e.what() << '\n';
					FD_CLR(index, &active_fs);
					delete connections[index];
					connections[index] = 0;
				}
				catch(const std::exception& e)
				{
					std::cerr << "errno: ";
					std::cerr << e.what() << '\n';
				}
			}
		}
	// TO DO timeout for http
	}
	// TO DO delete connections
}

Runnable* HttpWorker::clone() const
{
	return new HttpWorker(*this);
}
