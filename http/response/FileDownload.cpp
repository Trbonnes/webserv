#include "FileDownload.hpp"

FileDownload::FileDownload(ResponseContext& ctx, BufferChain& writeChain, struct stat *file) : HttpResponse(ctx)
{
	int fd;

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
        throw HttpError(INTERNAL_SERVER_ERROR);
    }
	_streamReadFd = fd;
    _state.read = DONE;
    _state.readStream = READY;
}

FileDownload::~FileDownload()
{

}