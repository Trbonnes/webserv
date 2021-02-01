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
			ListenSocketException(std::string msg, int errcode);
			virtual const char * what () const throw ();
			#ifdef __linux__
    			virtual ~ListenSocketException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
			#else
    			virtual ~ListenSocketException() _NOEXCEPT;
			#endif

	};
	class BindingException : public ListenSocketException
	{
		public:
			BindingException(int errcode);
			#ifdef __linux__
    			virtual ~BindingException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
			#else
    			virtual ~BindingException() _NOEXCEPT;
			#endif
	};
	class ListenException : public ListenSocketException
	{
		public:
			ListenException(int errcode);
			#ifdef __linux__
    			virtual ~ListenException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
			#else
    			virtual ~ListenException() _NOEXCEPT;
			#endif
	};

};

#endif // LISTENSOCKET
