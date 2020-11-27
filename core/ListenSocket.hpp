#if !defined(LISTENSOCKET)
#define LISTENSOCKET

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <string>
#include <string.h>

class ListenSocket
{
private:
	int	_sock;
	int _port;

public:
	ListenSocket(int port);
	ListenSocket(ListenSocket &&) = default;
	ListenSocket(const ListenSocket &) = default;
	ListenSocket &operator=(ListenSocket &&) = default;
	ListenSocket &operator=(const ListenSocket &) = default;
	~ListenSocket();
	int	getSock();
	int	getPort();

	class ListenSocketException : public std::exception
	{
		protected:
			std::string _msg;
		public:
			ListenSocketException(std::string msg, int errcode);
			virtual const char * what () const throw ();
			virtual ~ListenSocketException();

	};
	class BindingException : public ListenSocketException
	{
		public:
			BindingException(int errcode);
	};
	class ListenException : public ListenSocketException
	{
		public:
			ListenException(int errcode);
	};

};

#endif // LISTENSOCKET
