#include "Error.hpp"

HeadersError::HeadersError(ConfigServer *config , HttpRequest*request, std::string& route, std::string& location, BufferChain& writeChain, int code) : HttpResponse(config, request, route, location)
{
	BufferChain fakeWriteChain;
	HttpResponse* subError = new Error(config, request, route, location, fakeWriteChain, code);
	writeChain.pushBack(fakeWriteChain.getFirst());
	fakeWriteChain.popFirst();
	fakeWriteChain.flush();
	delete subError;
	_state.read = IGNORE; //TO DO shouldn't have to put this each time
}

void	HeadersError::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
	(void) writeChain;

	HttpResponse::handleRead(readChain, writeChain);
	_state.read = IGNORE;
}

void	HeadersError::handleWrite(BufferChain& readChain, BufferChain& writeChain)
{
	(void) readChain; // TO DO might simplify this ? to avoid all those void cast
	std::cout << "---------------------------------------------------------- " << writeChain <<std::endl;
	if (writeChain.getFirst() == NULL)
		throw HttpResponse::ConnectionClose();
}

HeadersError::~HeadersError()
{

}
