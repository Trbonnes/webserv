#include "FileDownload.hpp"

FileDownload::FileDownload(ConfigServer* config, HttpRequest* request,std::string route, std::string location, BufferChain& writeChain, struct stat *file) : HttpResponse(config, request, route, location)
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
        std::cout << "FILE ERROR" << std::endl;
	}
	_streamReadFd = fd;
    std::cout << "New file fd" << _streamReadFd << std::endl;
    _state.read = DONE;
    _state.readStream = READY;
}

FileDownload::~FileDownload()
{

}