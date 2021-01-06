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

	// TO DO not sure about those extension's names
	class ListenSocketException : public std::exception 
	{
		protected:
			std::string _msg;
		public:
			ListenSocketException(std::string msg, int errcode);
			virtual const char * what () const throw ();
    		virtual ~ListenSocketException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;

	};
	class BindingException : public ListenSocketException
	{
		public:
			BindingException(int errcode);
    		virtual ~BindingException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
	};
	class ListenException : public ListenSocketException
	{
		public:
			ListenException(int errcode);
    		virtual ~ListenException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
	};

};

#endif // LISTENSOCKET
