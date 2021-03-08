#include "HeadersOnly.hpp"


// HEAD
HeadersOnly::HeadersOnly(ConfigServer* config, HttpRequest* request, BufferChain& writeChain, std::string route, std::string location, struct stat* file)
{
	int fd;

	_location = location;
	if (S_ISREG(file->st_mode))
	{
		if ((fd = open(_route.c_str(), O_RDONLY)) != -1)
		{
			setLastModified(file);
			setContentType();
			setCharset();
			_contentLength = file->st_size;
			setServerName();
			setContentLocation();
			std::string* buff;
			buff = getRawHeaders();
			writeChain.pushBack(buff);
		} 
		else
		{
			// TO DO throw error
		}
	}
	else if (S_ISDIR(file->st_mode))
	{
		try
		{
			_subResponse = new FolderIndex(config, request, route, _fakeWriteChain, file);
			writeChain.pushBack(_fakeWriteChain.getFirst());
			_fakeWriteChain.popFirst();
			_fakeWriteChain.flush();
		}
		catch(const std::exception& e)
		{
			// std::cerr << e.what() << '\n';
			// TO DO throw error
		}
	}
	_state.write = WAITING;
}

void	HeadersOnly::handleRead(BufferChain& readChain)
{
	HttpResponse::handleRead(readChain);
	_streamWriteChain.flush();
	if (_state.read == DONE)
		_state.write = READY;
}


HeadersOnly::~HeadersOnly()
{
}

