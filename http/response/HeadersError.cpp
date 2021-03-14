#include "Error.hpp"

HeadersError::HeadersError(ConfigServer *config , HttpRequest*request, std::string& route, std::string& location, BufferChain& writeChain, int code) : HttpResponse(config, request, route, location)
{
	BufferChain fakeWriteChain;
	HttpResponse* subError = new Error(config, request, route, location, fakeWriteChain, code);
	writeChain.pushBack(fakeWriteChain.getFirst());
	fakeWriteChain.popFirst();
	fakeWriteChain.flush();
	delete subError;
}

void	HeadersError::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
	(void) writeChain;

	HttpResponse::handleRead(readChain, writeChain);
	_streamWriteChain.flush();
}

HeadersError::~HeadersError()
{

}
