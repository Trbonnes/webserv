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
		FD_SET(c->getSock(), &_active_read);
		// std::cerr << "New connection " << c->getSock() << std::endl;
	}
	catch(const std::exception& e)
	{
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
		if (select(FD_SETSIZE, &read_fs, &write_fs, NULL, NULL) == -1)
		{
			if (g_server->getStatus() == HttpServer::STOPPING)
				break;
			else
				std::cerr << "Select error: " << strerror(errno) << std::endl;
		}
		if (g_server->getStatus() == HttpServer::STOPPING)
				break;
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
				if (c->isStreamWriteReady(&write_fs))
					c->streamWrite();
				if (c->isWriteReady(&write_fs))
					c->write();
				if (c->isStreamReadReady(&read_fs))
					c->streamRead();
				if (c->isReadReady(&read_fs))
					c->read();
			}
			catch(const std::exception& e)
			{
				Log::debug("Connection close");
				ic = _connections.erase(ic);
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
