#include "ListenSocket.hpp"

ListenSocket::ListenSocket(int port) {
	_port = port;
    _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0)
	{
		// TO DO throw error
	}
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)); // TO DO WItchcraft
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port ); // program from configuration
	int addrlen = sizeof(address);
	(void) addrlen;

	int flags = fcntl(_sock, F_GETFL, 0);
	if (flags == -1)
	{
		// TO DO throw error
	}
	flags |= O_NONBLOCK;
	if (fcntl(_sock, F_SETFL, flags) == -1)
	{
		// TO DO thrwo error
	}

	if (bind(_sock, (struct sockaddr *)&address,
                                 sizeof(address)))
		throw BindingException(errno);
	if (listen(_sock, 3))
		throw ListenException(errno);
}

ListenSocket::ListenSocket(const ListenSocket &s)
{
	_sock = s._sock;
	_port = s._port;
}

ListenSocket &ListenSocket::operator=(const ListenSocket &s)
{
	_sock = s._sock;
	_port = s._port;
	return *this;
}

ListenSocket::~ListenSocket() {
}

int ListenSocket::getSock() {
    return _sock;
}

int ListenSocket::getPort() {
    return _port;
}

ListenSocket::ListenSocketException::ListenSocketException(std::string msg, int errcode)
{
	_msg = msg + " : " + strerror(errcode);
}

const char * ListenSocket::ListenSocketException::what () const throw ()
{
	return _msg.c_str();
}

ListenSocket::BindingException::BindingException(int errcode) : ListenSocketException("Socket binding error", errcode)
{
}

ListenSocket::ListenException::ListenException(int errcode) : ListenSocketException("Listen init error", errcode)
{
}

ListenSocket::ListenSocketException::~ListenSocketException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
}

ListenSocket::ListenException::~ListenException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
}

ListenSocket::BindingException::~BindingException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
}
