#if !defined(HEADERSONLY_HPP)
#define HEADERSONLY_HPP

#include "HttpResponse.hpp"

class HeadersOnly : public HttpResponse
{
private:
	BufferChain		_fakeWriteChain;
	HttpResponse*	_subResponse;
public:
	//Other
	HeadersOnly(ConfigServer* config, HttpRequest* request, std::string& route, std::string& location);

	// HEAD
	HeadersOnly(ConfigServer* config, HttpRequest* request, std::string& route, std::string& location, BufferChain& writeChain, struct stat* file);
	// POST
	HeadersOnly(ConfigServer* config, HttpRequest* request, std::string& route, std::string& location, BufferChain& writeChain, std::string& method);
	// OPTIONS
	// HeadersOnly(ConfigServer*, HttpRequest*, BufferChain& writeChain, std::string route, std::string location, );
	virtual ~HeadersOnly();
	void	handleRead(BufferChain& readChain, BufferChain& writeChain);
};

class FileDelete : public HeadersOnly
{

public:
	//Coplien to do
	FileDelete(ConfigServer* config, HttpRequest* request,std::string route, std::string location, BufferChain& writeChain);
	~FileDelete();
};

#endif // HEADERSONLY_HPP
