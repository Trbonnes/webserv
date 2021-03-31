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
	HeadersOnly(ResponseContext& ctx);

	// HEAD
	HeadersOnly(ResponseContext& ctx, BufferChain& writeChain, struct stat* file);
	// POST
	HeadersOnly(ResponseContext& ctx, BufferChain& writeChain, std::string& method);
	void	handleRead(BufferChain& readChain, BufferChain& writeChain);

	~HeadersOnly();
};

class FileDelete : public HeadersOnly
{

public:
	//Coplien to do
	FileDelete(ResponseContext& ctx, BufferChain& writeChain);
	~FileDelete();
};

#endif // HEADERSONLY_HPP
