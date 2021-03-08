#if !defined(HEADERSONLY_HPP)
#define HEADERSONLY_HPP

#include "HttpResponse.hpp"

class HeadersOnly : public HttpResponse
{
private:
	BufferChain		_fakeWriteChain;
	HttpResponse*	_subResponse;
public:
	// HEAD
	HeadersOnly(ConfigServer*, HttpRequest*, BufferChain& writeChain, std::string route, std::string location, struct stat*);
	// OPTIONS
	// HeadersOnly(ConfigServer*, HttpRequest*, BufferChain& writeChain, std::string route, std::string location, );
	~HeadersOnly();
	void	handleRead(BufferChain&);
};
#endif // HEADERSONLY_HPP
