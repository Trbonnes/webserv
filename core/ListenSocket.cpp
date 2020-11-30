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

	if (bind(_sock, (struct sockaddr *)&address,
                                 sizeof(address)))
		throw BindingException(errno);
	if (listen(_sock, 3))
		throw ListenException(errno);
}

ListenSocket::ListenSocket(const ListenSocket &)
{
}

ListenSocket &ListenSocket::operator=(const ListenSocket &)
{
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