#include "FileDownload.hpp"

FileDownload::FileDownload(ConfigServer* config, HttpRequest* request,std::string route, std::string location, BufferChain& writeChain, struct stat *file) : HttpResponse(config, request, route)
{
	int fd;

	_location = location;

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
	_streamReadFd = fd;
    _state.read = DONE;
    _state.readStream = READY;
}

FileDownload::~FileDownload()
{

}