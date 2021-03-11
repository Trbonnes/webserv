#include "HeadersOnly.hpp"


// HEAD
HeadersOnly::HeadersOnly(ConfigServer* config, HttpRequest* request, BufferChain& writeChain, std::string route, std::string location, struct stat* file) : HttpResponse(config, request, route) 
{
	_location = location;
	if (S_ISREG(file->st_mode))
	{
		try
		{
			_subResponse = new FileDownload(config, request, route, location, _fakeWriteChain, file);
		}
		catch(const std::exception& e)
		{
			// std::cerr << e.what() << '\n';
			// TO DO throw error
		}
	}
	else if (S_ISDIR(file->st_mode))
	{
		try
		{
			_subResponse = new FolderIndex(config, request, route, _fakeWriteChain, file);
		}
		catch(const std::exception& e)
		{
			// std::cerr << e.what() << '\n';
			// TO DO throw error
		}
	}
	delete _subResponse;
	writeChain.pushBack(_fakeWriteChain.getFirst());
	_fakeWriteChain.popFirst();
	_fakeWriteChain.flush();
	_state.write = READY;
}

HeadersOnly::HeadersOnly(ConfigServer* config, HttpRequest* request, BufferChain& writeChain, std::string route) : HttpResponse(config, request, route)
{
	_contentLength = 0;
	writeChain.pushBack(getRawHeaders());
	if (request->getContentLength() <= 0 && request->getTransferEncoding() != "chunked\r") // TO DO why ? 
		_state.write = READY;
	else
		_state.write = WAITING;
}

void	HeadersOnly::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
	HttpResponse::handleRead(readChain, writeChain);
	_streamWriteChain.flush();
	if (_state.read == DONE)
		_state.write = READY;
}


HeadersOnly::~HeadersOnly()
{
}

