#if !defined(CONNECTION_HPP)
#define CONNECTION_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "BufferChain.hpp"
#include "ListenSocket.hpp"
#include "HTTP/HttpRequest.hpp"
#include "HTTP/Http.hpp"
#include "HTTP/HTTP.hpp"


// The Connection class represents the interface between the server and the client
class Connection
{
private:
	// server <-> client socket
	FD				_socket;

	FD				_stream_read;
	FD				_stream_write;
	//client addr structure
	struct sockaddr	_client_name;
	
	// The http module, it processes the imcoming read
	Http			_module;

	// Reference to the worker sets
	fd_set*			_active_read;
	fd_set*			_active_write;

	// Pointers to socket buffer's chain
	BufferChain		_read_chain;
	BufferChain		_write_chain;

public:

	// Coplien
	Connection(int, fd_set* r, fd_set* w);
	Connection(const Connection&);
	Connection &operator=(const Connection &);
	~Connection();

	// Functions a module will use to sub write/read event to the workers select() fd sets
	void subWrite(int);
	void subRead(int);
	void subStreamWrite(int);
	void subStreamRead(int);

	// Functions to check if fds are active
	int	isWriteReady();
	int	isReadReady();
	int	isStreamWriteReady();
	int	isStreamReadReady();
	
	//Functions to call module's write and read operation
	int	write();
	int	read();
	int	streamWrite();
	int	streamRead();

	// Getters
	int getSock();
	BufferChain& getWriteChain();
	BufferChain& getReadChain();
	httpRequest *getSocket();
	
	// Setters
	void setSocket(httpRequest *s);
	void clearSocket();

	// Close the connection
	void close();
	
	// Exceptions
	class	Connectionfailed: public std::exception {
		public:
			virtual const char* what() const throw();
	};
};


#endif // CONNECTION_HPP
