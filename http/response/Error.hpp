#if !defined(ERROR_HPP)
#define ERROR_HPP

#include "HttpResponse.hpp"

class Error : public HttpResponse
{
public:
	Error(ConfigServer*, HttpRequest*, BufferChain&, int);
	~Error();

	void	handleRead(BufferChain&);
	void	handleWrite(BufferChain&);

};

class HeadersError : public HttpResponse
{
public:
	HeadersError(ConfigServer*, HttpRequest*, BufferChain&, int);
	~HeadersError();

	void	handleRead(BufferChain&);
	void	handleWrite(BufferChain&);
};

#endif // ERROR_HPP
