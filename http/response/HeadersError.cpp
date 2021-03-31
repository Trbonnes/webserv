#include "Error.hpp"

HeadersError::HeadersError(ResponseContext& ctx, BufferChain& writeChain, int code) : HttpResponse(ctx)
{
	BufferChain fakeWriteChain;
	HttpResponse* subError = new Error(ctx, fakeWriteChain, code);
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
