#if !defined(CONNECTION_HPP)
#define CONNECTION_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "ListenSocket.hpp"

#include "../HTTP/Socket.hpp"
#include "../HTTP/HTTP.hpp"


class Connection
{
private:
	int				_socket;
	struct sockaddr	_client_name;
	Socket*			_request;
	HTTP*			_response;

public:
	Connection(int);
	Connection(const Connection&);
	Connection &operator=(const Connection &);
	~Connection();
	int getSock();
	void close();
	Socket *getSocket();
	void setSocket(Socket *s);
	HTTP* getMethod();
	void setMethod(HTTP*);
	void clearSocket();
	class	Connectionfailed: public std::exception {
		public:
			virtual const char* what() const throw();
	};
};


#endif // CONNECTION_HPP
