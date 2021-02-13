#include "ListenSocket.hpp"

ListenSocket::ListenSocket(int port) {
	int opt;
	struct sockaddr_in address;

	_port = port;
    _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0)
	{
		throw SocketCreationException(errno);
	}
	opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port ); // program from configuration
	if (fcntl(_sock, F_SETFL, O_NONBLOCK) == -1)
	{
		throw FcntlException(errno);
	}

	if (bind(_sock, (struct sockaddr *)&address,
                                 sizeof(address)))
		throw BindingException(errno);
	if (listen(_sock, 1024))
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

ListenSocket::FcntlException::FcntlException(int errcode) : ListenSocketException("Fcntl error", errcode)
{
}

ListenSocket::SocketCreationException::SocketCreationException(int errcode) : ListenSocketException("Socket creation error", errcode)
{
}
