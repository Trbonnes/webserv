#include "Error.hpp"

HeadersError::HeadersError(ConfigServer *config , HttpRequest*request, BufferChain& writeChain, int code)
{
	BufferChain fakeWriteChain;
	HttpResponse* subError = new Error(config, request, fakeWriteChain, code);
	writeChain.pushBack(fakeWriteChain.getFirst());
	fakeWriteChain.popFirst();
	fakeWriteChain.flush();
	delete subError;
	_state.read = IGNORE; //TO DO shouldn't have to put this each time
}

void	HeadersError::handleRead(BufferChain& readChain)
{
	HttpResponse::handleRead(readChain);
	_state.read = IGNORE;
}

void	HeadersError::handleWrite(BufferChain& writeChain)
{
	std::cout << "---------------------------------------------------------- " << writeChain <<std::endl;
	if (writeChain.getFirst() == NULL)
		throw HttpResponse::ConnectionClose();
}

HeadersError::~HeadersError()
{

}

