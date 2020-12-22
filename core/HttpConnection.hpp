#if !defined(HTTPCONNECTION)
#define HTTPCONNECTION

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "ListenSocket.hpp"

class HttpConnection
{
public:
	HttpConnection(ListenSocket& listen_sock);
	~HttpConnection();

private:
	HttpConnection &operator=(const HttpConnection &);
	HttpConnection(const HttpConnection &);
	int				_sock;
	ListenSocket&	_listen_sock;
	struct sockaddr	_client_name;
	char			_buff[CONNECTION_BUFF_SIZE];

public:
	void	accept();
	void	write(char *data, size_t size);
	int		getSock();
	int		getPort();
	
	class ConnectionClose : public std::exception
	{
		public:
			const char * what () const throw ();
	};
	class AcceptFailed : public std::exception
	{
		public:
			const char * what () const throw ();
	};
};

#endif // HTTPCONNECTION
