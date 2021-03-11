#if !defined(ERROR_HPP)
#define ERROR_HPP

#include "HttpResponse.hpp"

class Error : public HttpResponse
{
public:
	Error(ConfigServer*, HttpRequest*, std::string& route, std::string& location, BufferChain&, int);
	~Error();

	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
	void	handleWrite(BufferChain& readChain, BufferChain& writeChain);

};

class HeadersError : public HttpResponse
{
public:
	HeadersError(ConfigServer*, HttpRequest*, BufferChain&, int);
	~HeadersError();

	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
	void	handleWrite(BufferChain& readChain, BufferChain& writeChain);
};

#endif // ERROR_HPP
