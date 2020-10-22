#if !defined(HTTPCONNECTION)
#define HTTPCONNECTION

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
	struct sockaddr	client_name;

public:
	void	accept(int connection_sock);
	void	write(void *data, size_t size);
	int		getSock();
	
};

#endif // HTTPCONNECTION
