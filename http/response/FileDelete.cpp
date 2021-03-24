#include "HeadersOnly.hpp"


FileDelete::FileDelete(ConfigServer* config, HttpRequest* request,std::string route, std::string location, BufferChain& writeChain) : HeadersOnly(config, request, route, location)
{
	struct stat file;

	// Try to get stat of route
	// if it returns anything other than 0, assume it's not found
	if (stat(_route.c_str(), &file))
	{
		if (errno == ENOENT)
			throw HttpError(NOT_FOUND);
		if (errno == EACCES)
			throw HttpError(FORBIDDEN);
		throw HttpError(INTERNAL_SERVER_ERROR);
	}
	else
	{
		int ret = -1;
		//try to unlink file
		if ((file.st_mode & S_IFMT) == S_IFREG)
				ret = unlink(_route.c_str());
		// try to rm dir
		else if (((file.st_mode & S_IFMT) == S_IFDIR))
				ret = rmdir(_route.c_str());
		// if one of those fails no content
		if (ret == -1)
				throw HttpError(FORBIDDEN);
		_statusCode = NO_CONTENT;
	}
	if (_state.read == DONE)
	{
		writeChain.pushBack(getRawHeaders());
		_state.write = READY;
	}
}

FileDelete::~FileDelete()
{

}
