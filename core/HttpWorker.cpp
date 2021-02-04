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
void	HttpWorker::writeResponse(Connection *c)
{
	char *response;
	size_t responseSize;

	if (equalRequest(c->getRequest(), _cacheSocket) == 0)
	{
		c->clearRequest();
		response = _cacheResponse;
		responseSize = _cacheResponseSize;
	}
	else
	{
		if (_cacheResponse != NULL)
		{
			free(_cacheResponse);
			_cacheResponse = NULL;
		}
		if (_cacheSocket != NULL)
		{
			delete _cacheSocket;
			_cacheSocket = NULL;
		}
		ConfigServer *configServer = NULL;
		configServer = _config->getServerUnit(c->getRequest()->getPort(), c->getRequest()->getHost());
		if (configServer == NULL)
				throw Socket::ConnectionClose(); // TO DO put in a try catch block
		HTTP method(c->getRequest(), configServer); 
		response = method.getResponse();
		responseSize = method.getResponseSize();
	}

	
	
	if (write(c->getSock(), response, responseSize) == -1)
	{
		std::cerr << "Tried to write but didn't work" << std::endl;
	} // TO DO check error and close the connection

	// if (c->getRequest()->getMethod().compare("POST") == 0)
	// {
	// 	free(response);
	// 	response = NULL;
	// 	delete newSocket;
	// 	newSocket = NULL;
	// }
	_cacheSocket = c->getRequest();
	_cacheResponse = response;
	_cacheResponseSize = responseSize;
	c->setRequest(NULL);
	FD_SET(c->getSock(), &_active_read);
	FD_CLR(c->getSock(), &_active_write);
}

void	HttpWorker::acceptConnection(int s)
{
	try
	{
		Connection *c = new Connection(s);
		_connections.push_front(c);
		FD_SET(c->getSock(), &_active_read);
		// std::cerr << "New connection " << c->getSock() << std::endl;
	}
	catch(const std::exception& e)
	{
	}
}

void	HttpWorker::readRequest(Connection *c)
{
		Socket *newSocket = httpRequestParser(c->getSock());
		c->setRequest(newSocket);
		FD_SET(c->getSock(), &_active_write);
		FD_CLR(c->getSock(), &_active_read);
}

void	HttpWorker::run()
{
	// FD_SETS
	fd_set 	write_fs = { 0 };
	fd_set 	read_fs = { 0 };

	//Cache sockt init
	_cacheSocket = new Socket();
	_cacheResponse = NULL;
	_cacheResponseSize = 0;

	FD_ZERO(&_active_read);
	FD_ZERO(&_active_write);
	//List iterators;
	std::list<ListenSocket>::iterator il;
	std::list<Connection*>::iterator ic;

	// add Listen sockets in _active_read
	for (il = _listen_socket.begin(); il != _listen_socket.end(); il++)
		FD_SET(il->getSock(), &_active_read);

	// Main loop
	while (1)
	{
		read_fs = _active_read;
		write_fs = _active_write;

		if (select(FD_SETSIZE, &read_fs, &write_fs, NULL, NULL) == -1)
		{
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			// TO DO ERROR
		}

		// New connection
		for (il = _listen_socket.begin(); il != _listen_socket.end(); il++)
			if (FD_ISSET(il->getSock(), &read_fs))
				acceptConnection(il->getSock());

		ic = _connections.begin();
		while (ic != _connections.end())
		{
			Connection* c = *ic;
			try
			{
				// Read ready on active connection
				if (FD_ISSET(c->getSock(), &read_fs))
					readRequest(c);
				// Write ready on active connection
				else if (FD_ISSET(c->getSock(), &write_fs))
					writeResponse(c);		
			}
			catch(const std::exception& e)
			{
				FD_CLR(c->getSock(), &_active_write);
				FD_CLR(c->getSock(), &_active_read);
				c->clearRequest();
				ic = _connections.erase(ic);
				delete c;
				continue;
			}
			ic++;
		}
	}
}

Runnable* HttpWorker::clone() const
{
	return new HttpWorker(*this);
}
