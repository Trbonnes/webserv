#if !defined(ERROR_HPP)
#define ERROR_HPP

#include "HttpResponse.hpp"

class Error : public HttpResponse
{
public:
	Error(ResponseContext&, BufferChain&, int);
	~Error();

	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
};

class HeadersError : public HttpResponse
{
public:
	HeadersError(ResponseContext&, BufferChain&, int);
	~HeadersError();

	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
};

#endif // ERROR_HPP
