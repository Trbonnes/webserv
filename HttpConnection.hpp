#if !defined(HTTPCONNECTION)
#define HTTPCONNECTION

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class HttpConnection
{
public:
	HttpConnection() = default;
	HttpConnection(HttpConnection &&) = default;
	HttpConnection(const HttpConnection &) = default;
	HttpConnection &operator=(HttpConnection &&) = default;
	HttpConnection &operator=(const HttpConnection &) = default;
	~HttpConnection() = default;

private:
	int				_sock;
	struct sockaddr	_client_name;

public:
	void	acceptOnSocket(int connection_sock);
	void	write(char *data, size_t size);
	void	read();
	int		getSock();
	
};

#endif // HTTPCONNECTION
