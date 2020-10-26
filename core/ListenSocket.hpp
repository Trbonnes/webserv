#if !defined(LISTENSOCKET)
#define LISTENSOCKET

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>

class ListenSocket
{
public:
	ListenSocket();
	ListenSocket(ListenSocket &&) = default;
	ListenSocket(const ListenSocket &) = default;
	ListenSocket &operator=(ListenSocket &&) = default;
	ListenSocket &operator=(const ListenSocket &) = default;
	~ListenSocket();

private:
	int	_sock;
	// add configuration
public:
	int	getSock();

};

#endif // LISTENSOCKET
