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

void	HttpWorker::acceptConnection(int s)
{
	try
	{
		Connection *c = new Connection(s, &_active_read, &_active_write, _config);
		_connections.push_front(c);
		std::cout << "Connection pointer value" << c << std::endl;
		FD_SET(c->getSock(), &_active_read);
		// std::cerr << "New connection " << c->getSock() << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "Couldn't open connection : " << e.what() << std::endl;
	}
}

void	HttpWorker::run()
{
	// FD_SETS
	fd_set 	write_fs = { 0 };
	fd_set 	read_fs = { 0 };

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
		Log::debug("Select");
		std::cout << "================================================= SELECT" << std::endl;
		if (select(FD_SETSIZE, &read_fs, &write_fs, NULL, NULL) == -1)
		{
			if (g_server->getStatus() == HttpServer::STOPPING)
				break;
			else
				std::cerr << "oooooooooooooooooooooooooooooooooooooooo>>> Select error: " << strerror(errno) << std::endl;
		}
		Log::debug("After select");
		if (g_server->getStatus() == HttpServer::STOPPING)
				break;
		// New connection
		for (il = _listen_socket.begin(); il != _listen_socket.end(); il++)
			if (FD_ISSET(il->getSock(), &read_fs))
			{
				std::cout << "New Connection !" << std::endl;
				acceptConnection(il->getSock());
			}
		ic = _connections.begin();
		while (ic != _connections.end())
		{
			Connection* c = *ic;
			std::cout << "=== Connection Event "  << c << std::endl;
			try
			{
				// std::cout << "=============== status : " << FD_ISSET(63, &_active_read) << std::endl;
				std::cout << c->isStreamWriteReady(&write_fs) << c->isWriteReady(&write_fs) << c->isStreamReadReady(&read_fs) << c->isReadReady(&read_fs) << std::endl;
				if (c->isStreamWriteReady(&write_fs))
					c->streamWrite();
				// std::cout << "=============== status : " << FD_ISSET(63, &_active_read) << std::endl;
				
				if (c->isWriteReady(&write_fs))
					c->write();
				// std::cout << "=============== status : " << FD_ISSET(63, &_active_read) << std::endl;
				
				if (c->isStreamReadReady(&read_fs))
					c->streamRead();
				// std::cout << "=============== status : " << FD_ISSET(63, &_active_read) << std::endl;
				if (c->isReadReady(&read_fs))
					c->read();
			}
			catch(const std::exception& e)
			{
				// Log::debug("Connection close");
				std::cout << "=== > Connection closed "  << c << std::endl;
				ic = _connections.erase(ic);
				std::cout << "Lenght : " << _connections.size() << std::endl;
				if (_connections.size() == 1)
				{
					std::cout << *_connections.begin() << std::endl;
				}
				delete c;
				continue;
			}
			ic++;
		}
	}
	closeConnections();
}

void		HttpWorker::closeConnections()
{
	std::list<Connection*>::iterator ic,next;

	ic = _connections.begin();
	while (ic != _connections.end())
	{
		next = ic;
		next++;
		delete *ic;
		_connections.erase(ic);
		ic = next;
	}
}

Runnable*	HttpWorker::clone() const
{
	return new HttpWorker(*this);
}
