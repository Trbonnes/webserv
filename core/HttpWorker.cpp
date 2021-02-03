#include "HttpWorker.hpp"
#include <sys/sysinfo.h>

HttpWorker::HttpWorker(std::list<ListenSocket> &listen, Config* config) : Runnable(1, 1)
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

int		equalRequest(Socket *newSocket, Socket *lastSocket)
{
	if (lastSocket == NULL)
		return 1;
;	if (newSocket->getMethod().compare(lastSocket->getMethod()))
		return 1;
	if (newSocket->getRequestURI().compare(lastSocket->getRequestURI()))
		return 1;
	if (newSocket->getHttpVersion().compare(lastSocket->getHttpVersion()))
		return 1;
	if (newSocket->getContentLength().compare(lastSocket->getContentLength()))
		return 1;
	if (newSocket->getContentLocation().compare(lastSocket->getContentLocation()))
		return 1;
	if (newSocket->getContentType().compare(lastSocket->getContentType()))
		return 1;
	if (newSocket->getTransferEncoding().compare(lastSocket->getTransferEncoding()))
		return 1;
	if (newSocket->getAuthorization().compare(lastSocket->getAuthorization()))
		return 1;
	if (newSocket->getHost().compare(lastSocket->getHost()))
		return 1;
	if (newSocket->getPort() != lastSocket->getPort())
		return 1;
	if (newSocket->getUserAgent().compare(lastSocket->getUserAgent()))
		return 1;
	if (newSocket->getReferer().compare(lastSocket->getReferer()))
		return 1;
	if (newSocket->getRemoteAddr().compare(lastSocket->getRemoteAddr()))
		return 1;
	if (newSocket->getXSecret().compare(lastSocket->getXSecret()))
		return 1;
	if  (newSocket->getAcceptCharset() != lastSocket->getAcceptCharset())
		return 1;	
	if  (newSocket->getAcceptLanguage() != lastSocket->getAcceptLanguage())
		return 1;
	if (newSocket->getBody().compare(lastSocket->getBody()))
		return 1;
	return 0;
}

void	HttpWorker::run()
{

	//MISC
	ConfigServer *configServer = NULL;
	// Cache request
	// Socket *lastSocket = new Socket();

	// FD_SETS
	fd_set 	write_fs = { 0 };
	fd_set 	read_fs = { 0 };
	fd_set 	active_read = { 0 };
	fd_set 	active_write = { 0 };

	//List iterators;
	std::list<ListenSocket>::iterator il;
	std::list<Connection*>::iterator ic, tmp;

	// add Listen sockets in active_read
	for (il = _listen_socket.begin(); il != _listen_socket.end(); il++)
	{
		FD_SET(il->getSock(), &active_read);
		std::cerr << "Listen socket on " << il->getSock() << std::endl;
	}
	

	// Main loop
	while (1)
	{
		read_fs = active_read;
		write_fs = active_write;

		// std::cerr << "=============== SELECT START ===============" << std::endl;
		// std::cerr << "Number of actives connections " << _connections.size() << std::endl;
		// usleep(80000);
		//SELECT
		int r = 0;
		// std::cerr << "FD_SETSIZE :" << FD_SETSIZE << std::endl;
		if ((r = select(FD_SETSIZE, &read_fs, &write_fs, NULL, NULL)) == -1)
		{
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			// TO DO ERROR
		}

		// std::cerr << "Loop: " << r << "fds to go" << std::endl;

		// New connection
		for (il = _listen_socket.begin(); il != _listen_socket.end(); il++)
			if (FD_ISSET(il->getSock(), &read_fs))
			{
				try
				{
					Connection *c = new Connection(il->getSock());
					_connections.push_front(c);
					FD_SET(c->getSock(), &active_read);
					// std::cerr << "New connection " << c->getSock() << std::endl;
				}
				catch(const std::exception& e)
				{
				}
			}
		ic = _connections.begin();
		while (ic != _connections.end())
		{
			Connection* c = *ic;
			// Read ready on active connection
			// std::cerr << "Socket for loop : " << ic->getSock() << std::endl;
			if (FD_ISSET(c->getSock(), &read_fs))
			{
				// std::cerr << "About to read " << c->getSock() << std::endl;
				Socket *newSocket;
				try
				{
					 newSocket = httpRequestParser(c->getSock());
				}
				catch(const std::exception& e)
				{
					FD_CLR(c->getSock(), &active_write);
					FD_CLR(c->getSock(), &active_read);
					c->clearRequest();
					// std::cerr << "Connection close for " << c->getSock() << std::endl;
					ic = _connections.erase(ic);
					delete c;
					continue;
				}
				c->setRequest(newSocket);
				FD_SET(c->getSock(), &active_write);
				FD_CLR(c->getSock(), &active_read);
				// std::cerr << "READ"<< std::endl;
			}
			// Write ready on active connection
			else if (FD_ISSET(c->getSock(), &write_fs))
			{
				// std::cerr << "About to write on " << c->getSock() << std::endl;
				configServer = _config->getServerUnit(c->getRequest()->getPort(), c->getRequest()->getHost());
				if (configServer == NULL)
						throw Socket::ConnectionClose(); // TO DO put in a try catch block
				HTTP method(c->getRequest(), configServer); 
				char * response = method.getResponse();
				size_t responseSize = method.getResponseSize();
				if (write(c->getSock(), response, responseSize) == -1)
				{
					std::cerr << "Tried to write but didn't work" << std::endl;
				} // TO DO check error and close the connection
				FD_SET(c->getSock(), &active_read);
				FD_CLR(c->getSock(), &active_write);
				c->clearRequest();
				// std::cerr << "WRITE"<< std::endl;
			}
			ic++;
		}
	}





	// Socket *lastSocket = new Socket();

	// fd_set 	active_fs;
	// fd_set 	read_fs;
	// char* 	response;
	// int		responseSize;
	// ListenSocket* 	listening[FD_SETSIZE]; // array of pointers
	// ConfigServer *configServer = NULL;
	// response = NULL;


	// std::list<int>

	// // Important zeroing-out of the arrays
	// // ft_bzero(connections, FD_SETSIZE * sizeof(HttpConnection*)); 
	// ft_bzero(listening, FD_SETSIZE * sizeof(ListenSocket*));

	// // Transforming list in fdset and ListenSocket in an array we can access with i directly without looping through the fdset
	// FD_ZERO(&active_fs);
	// for (unsigned int i = 0; i < _listen_socket.size(); i++)
	// {
	// 	listening[_listen_socket[i].getSock()] = &_listen_socket[i];
	// 	FD_SET(_listen_socket[i].getSock(), &active_fs);
	// }
	// // Main loop of the worker
	// while (1)
	// {
	// 	//read fs is going to be modified by select call, so we must reattribute the set there
	// 	read_fs = active_fs;
	// 	// Waiting for an event on listen socket
	// 	if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1)
	// 	{
	// 		std::cerr << "Select error : " << strerror(errno) << std::endl;
	// 		continue;
	// 	}
	// 	for (int i = 0; i < FD_SETSIZE; i++)
	// 	{
	// 		// if the fd is not set then there's no event on that fd, next
	// 		if (!FD_ISSET(i, &read_fs))
	// 			continue ;
	// 		// if it is on a listening socket, create a new connection
	// 		if (listening[i])
	// 		{	
	// 			int s;

	// 			struct sockaddr	_client_name;
	// 			socklen_t size;

	// 			size = sizeof(_client_name);
	// 			s = ::accept(listening[i]->getSock(), &_client_name, &size);
	// 			if (s != -1)
	// 			{
	// 				if (s < FD_SETSIZE)
	// 				{
	// 					FD_SET(s, &active_fs);
	// 				}
	// 				else
	// 					close(s);
	// 			}
	// 			else
	// 			{
	// 				close(s);
	// 			}
				
	// 		}
	// 		// If it is a connection socket, do the job
	// 		else
	// 		{
	// 			try
	// 			{
	// 				Socket *newSocket = httpRequestParser(i);
	// 				if (equalRequest(newSocket, lastSocket))
	// 				{
	// 					if (response != NULL)
	// 					{
	// 						free(response);
	// 						response = NULL;
	// 					}
	// 					if (lastSocket != NULL)
	// 					{
	// 						delete lastSocket;
	// 						lastSocket = NULL;
	// 					}
	// 					configServer = _config->getServerUnit(newSocket->getPort(), newSocket->getHost());
	// 					if (configServer == NULL)
	// 						throw Socket::ConnectionClose();
	// 					HTTP method(newSocket, configServer);
	// 					response = method.getResponse();
	// 					responseSize = method.getResponseSize();
	// 					write(i, response, responseSize);

	// 					if (newSocket->getMethod().compare("POST") == 0)
	// 					{
	// 						free(response);
	// 						response = NULL;
	// 						delete newSocket;
	// 						newSocket = NULL;
	// 					}
	// 					lastSocket = newSocket;
	// 				}
	// 				else
	// 				{
	// 					delete newSocket;
	// 					newSocket = NULL;
	// 					write(i, response, responseSize);
	// 				}
	// 			}
	// 			catch(const std::exception& e)
	// 			{
	// 				close(i);
	// 				FD_CLR(i, &active_fs);
	// 			}
	// 		}
	// 	}
	// }
	// std::cerr << "EXITING WORKER " << std::endl;
	// exit(0);
}

Runnable* HttpWorker::clone() const
{
	return new HttpWorker(*this);
}
