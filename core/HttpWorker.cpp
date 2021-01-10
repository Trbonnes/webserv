#include "HttpWorker.hpp"


//TO DO The runnable arguments might depend from the configuration file
HttpWorker::HttpWorker(std::vector<ListenSocket> &listen, Config* config) : Runnable(1, 1)
{
	_config = config;
    _listen_socket = listen;
}

HttpWorker::HttpWorker(const HttpWorker &w) : Runnable(w)
{
	_config = w._config;
    _listen_socket = w._listen_socket;
}

HttpWorker::~HttpWorker() {
}

int _testFILE = 0; // TEST

// TO DO Check config var regarding max connections and max port/server block
void	HttpWorker::run()
{
	fd_set 	active_fs;
	fd_set 	read_fs;
	char* 	response;
	int		responseSize;
	// HttpConnection*	new_connection; // temp pointer 
	bool	connections[FD_SETSIZE]; // array of connections
	ListenSocket* 	listening[FD_SETSIZE]; // array of pointers

	// Important zeroing-out of the arrays
	// ft_bzero(connections, FD_SETSIZE * sizeof(HttpConnection*)); 
	ft_bzero(connections, FD_SETSIZE * sizeof(bool)); 
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
			std::cerr << "Select error " << strerror(errno) << std::endl;
			continue; // TO DO throw something ?
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			// if the fd is not set then there's no event on that fd, next
			if (!FD_ISSET(i, &read_fs))
				continue ;
			// if it is on a listening socket, create a new connection
			if (listening[i])
			{
				int s;

				struct sockaddr	_client_name;
				socklen_t size;

				size = sizeof(_client_name);
				s = ::accept(listening[i]->getSock(), &_client_name, &size);
				if (s != -1 && s < FD_SETSIZE)
				{
					FD_SET(s, &active_fs);
					connections[s] = true;
				}
				else
					close(s);
			}
			// If it is a connection socket, do the job
			else if (connections[i])
			{
				// std::cout << "Event on connection" << std::endl;
				try
				{
					// std::cout << "New event" << std::endl;
					// Socket *socket = httpRequestParser(connections[i]->getSock()); // TO DO why would it return a socket class and not an httpRequest object ? 
					Socket *socket = httpRequestParser(i); // TO DO why would it return a socket class and not an httpRequest object ? 
					ConfigServer &ptr2 = _config->getServerList()[0];
					HTTP method(socket, ptr2);

					response = method.getResponse(); // TO DO make code more modulare and clean up names
					responseSize = method.getResponseSize();					
					// connections[i]->write(response, responseSize); // TO DO ugly
					write(i, response, responseSize); // TO DO ugly
				}
				catch(const std::exception& e)
				{
					// int index = connections[i]->getSock();
					// // std::cerr << "Connection has been closed: " << e.what() << '\n';
					// FD_CLR(index, &active_fs);
					// delete connections[index];
					// connections[index] = 0;
					close(i);
					connections[i] = false;
					FD_CLR(i, &active_fs);
				}
			}
		}
	// TO DO timeout for http
	}
	// TO DO delete connections
	std::cout << "EXITING WORKER " << std::endl;
	exit(0);
}

Runnable* HttpWorker::clone() const
{
	return new HttpWorker(*this);
}
