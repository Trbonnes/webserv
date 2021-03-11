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
	HeadersOnly(ConfigServer* config, HttpRequest* request, std::string& route, std::string& location, BufferChain& writeChain, struct stat* file);
	// POST
	HeadersOnly(ConfigServer* config, HttpRequest* request, std::string& route, std::string& location, BufferChain& writeChain);
	// OPTIONS
	// HeadersOnly(ConfigServer*, HttpRequest*, BufferChain& writeChain, std::string route, std::string location, );
	~HeadersOnly();
	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
};
#endif // HEADERSONLY_HPP
