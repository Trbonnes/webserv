#include "HttpWorker.hpp"


//TO DO The runnable arguments might depend from the configuration file
HttpWorker::HttpWorker(std::vector<ListenSocket> &listen, Config* config) : Runnable(1, 1)
{
	std::cout << "Worker Initializing" << std::endl;
	_config = config;
    _listen_socket = listen;
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
	// Important zeroing of the values
	ft_bzero(connections, FD_SETSIZE * sizeof(ListenSocket*)); 
	ft_bzero(listening, FD_SETSIZE * sizeof(ListenSocket*));
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
			continue; // TO DO throw something ?
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
					new_connection = new HttpConnection(*listening[i]);
					new_connection->accept();
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
				// char buff[408];
				// while (read(connections[i]->getSock(), buff, 408))
				// {
				// 	std::cout << buff << std::endl;
				// }
				std::cout << "ABOUT TO PARSE" << std::endl;
				
				try
				{
					Socket *socket = httpRequestParser(connections[i]->getSock()); // TO DO why would it return a socket class and not an httpRequest object ? 
					// ConfigServer *ptr = _config->getServerUnit(connections[i]->getSock(), socket->getHost()); // TO DO check if null ?
					// std::cout << "PASSED THIS POINT, value of ptr : " << ptr << socket->getHost() << std::endl;
					
					ConfigServer &ptr2 = _config->getServerList()[0];
					HTTP method(socket, ptr2);
					std::cout << "METHOD HAS BEEN CONSTRUCTED" << std::endl;		

<<<<<<< HEAD
				std::cout << "ABOUT TO CREATE RESPONSE" << std::endl;
				response = method.getResponse(); // TO DO make code more modulare and clean up names
				responseSize = method.getResponseSize();
				// std::cerr << responseSize << std::endl;
				// std::cerr << response << std::endl;
				std::cout << "RESPONSE CREATED" << std::endl;
				connections[i]->write(response, responseSize); // TO DO ugly
				std::cout << std::endl << "ENDING REQUEST" << std::endl;
=======
					std::cout << "ABOUT TO CREATE RESPONSE" << std::endl;
					response = method.getResponse(); // TO DO make code more modulare and clean up names
					responseSize = method.getResponseSize();
					std::cerr << "Response size: " << responseSize << std::endl;
					std::cerr << response << std::endl;
					std::cout << "RESPONSE CREATED" << std::endl;
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
>>>>>>> develop
			}
			else
			{
				// TO DO or not ?
			}
			// TO DO wero it just in case ?
			FD_ZERO(&read_fs);
		}
	// TO DO timeout for http
	}
	// TO DO delete connections
}
