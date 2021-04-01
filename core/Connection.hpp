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
#include "Config.hpp"
// HTTP Module
#include "http/Http.hpp"

// The Connection class represents the interface between the server and the client
// The read and write on a socket are performed by this class
// The input data is put in buffer chained for the http module to handle
class Connection
{
private:
	//client addr structure
	struct sockaddr	_client_name;
	

	// Reference to the worker sets
	fd_set*			_active_read;
	fd_set*			_active_write;

	// References to socket buffer's chain, used to add buffer to the soket write
	BufferChain	_read_chain;
	BufferChain	_write_chain;

	// The http module, it processes the imcoming read
	Http			_module;

	Connection();
	Connection(const Connection&);
	Connection &operator=(const Connection &);
	FD				_socket;
	FD				_stream_read;
	FD				_stream_write;

public:
	// server <-> client socket

	// Coplien
	Connection(int, fd_set* r, fd_set* w, Config* c);
	~Connection();

	// Functions a module will use to sub write/read event to the workers select() fd sets
	void subWrite();
	void subRead();
	void setStreamWrite(int fd);
	void subStreamWrite();
	void setStreamRead(int fd);
	void subStreamRead();
	// unsub
	void unsubWrite();
	void unsubRead();
	void unsubStreamWrite();
	void unsubStreamRead();


	// Functions to check if fds are active
	int	isWriteReady(fd_set* set);
	int	isReadReady(fd_set* set);
	int	isStreamWriteReady(fd_set* set);
	int	isStreamReadReady(fd_set* set);
	
	//Functions to call module's write and read operation
	void write();
	void read();
	void streamWrite();
	void streamRead();

	// Getters
	int getSock();
	BufferChain& getWriteChain();
	BufferChain& getReadChain();
	
	// Setters
	void clearSocket();

	// Close the connection
	void close();
	
	// Exceptions
	class	Connectionfailed: public std::exception {
		public:
			virtual const char* what() const throw();
	};
	class   ConnectionClose : public std::exception
	{
		public:
			const char * what () const throw ();
	};    
};


#endif // CONNECTION_HPP
