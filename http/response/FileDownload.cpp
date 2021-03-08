#include "FileDownload.hpp"

FileDownload::FileDownload(ConfigServer* config, HttpRequest* request,std::string route, std::string location, struct stat *file) : HttpResponse(config, request, route)
{
	int fd;

	_location = location;
	_file = file;


    if ((fd = open(_route.c_str(), O_RDONLY)) != -1)
    {
        setLastModified();
        setContentType();
        setCharset();
        _contentLength = _file->st_size;
        setServerName();
        setContentLocation();
    } 
	else
	{
		// to do throw error
	}
	_streamReadFd = fd;
}

void        FileDownload::setCharset(void)
{
    _charset = _config->getCharset(_location);
}

void        FileDownload::setLastModified()
{
    struct tm *timeinfo;

	#ifdef __linux__
    		timeinfo = localtime(&(_file->st_mtim.tv_sec));
	#else
            timeinfo = localtime(&(_file->st_mtimespec.tv_sec)); // st_mtimespec.tv_sec = macos ; st_mtim = linux
	#endif // TARGET_OS_MAC
    strftime(_lastModified, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
}


void        FileDownload::setContentType()
{
    int                     find;
    int                     length;
    std::map<std::string, std::string>::iterator it;

    find = _route.find_last_of('.');
    find += 1;
    length = _route.length() - find;
    _contentType = _route.substr(find, length);
    find = -1;
    it = _config->getMimeTypes().begin();

    while (it != _config->getMimeTypes().end())
    {
        if ((it->first).compare(_contentType) == 0)
            break;
        it++;
    }
    if (it != _config->getMimeTypes().end())
        _contentType = it->second;
    else
        _contentType = _config->getType(_location);
}

FileDownload::~FileDownload()
{

}