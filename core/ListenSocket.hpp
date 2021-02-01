#if !defined(LISTENSOCKET)
#define LISTENSOCKET

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#ifdef Q_OS_MAC
#undef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT _NOEXCEPT
#endif

class ListenSocket
{
private:
	int	_sock;
	int _port;

public:
	ListenSocket(int port);
	ListenSocket(const ListenSocket &);
	ListenSocket &operator=(const ListenSocket &);
	~ListenSocket();
	int	getSock();
	int	getPort();

	class ListenSocketException : public std::exception 
	{
		protected:
			std::string _msg;
		public:
			virtual ~ListenSocketException() throw() {return ;}
			ListenSocketException(std::string msg, int errcode);
			virtual const char * what () const throw ();

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
	class FcntlException : public ListenSocketException
	{
		public:
			FcntlException(int errcode);
	};
	class SocketCreationException : public ListenSocketException
	{
		public:
			SocketCreationException(int errcode);
	};
};

#endif // LISTENSOCKET
