#include "HttpResponse.hpp"

//** absolute location route for the user agent **
void        HttpResponse::setContentLocation()
{
    _contentLocation.assign("http://").append(_socket.getHost()).append("/").append(_route);
}

//** Copy file into body string **
void        HttpResponse::setBody(int fd)
{
    int     ret;

    _body = (char*)ft_calloc(_stat.st_size, sizeof(char));
    ret = read(fd, _body, _stat.st_size);
    if (ret == -1)
        _statusCode = INTERNAL_SERVER_ERROR;
}

void        HttpResponse::setCharset(void)
{
    // if (_contentType.compare("text/html") == 0)
    _charset = _config.getCharset(_location);
}

void        HttpResponse::setServerName()
{
    _server = _socket.getHost();
}

void        HttpResponse::setContentLength()
{
   _contentLength = _stat.st_size;
}

void        HttpResponse::setLastModified()
{
    struct tm *timeinfo;

	#ifdef __linux__
    		timeinfo = localtime(&(_stat.st_mtim.tv_sec));
	#else
            timeinfo = localtime(&(_stat.st_mtimespec.tv_sec)); // st_mtimespec.tv_sec = macos ; st_mtim = linux
	#endif // TARGET_OS_MAC
    strftime(_lastModified, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
}

void        HttpResponse::setDate()
{
    struct timeval  tv;
    struct tm       *timeinfo;

    if (gettimeofday(&tv, NULL) == 0)
    {
        timeinfo = localtime(&(tv.tv_sec));
        strftime(_date, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
    }
}

void        HttpResponse::setContentType()
{
    int                     find;
    int                     length;
    std::map<std::string, std::string>::iterator it;

    find = _route.find_last_of('.');
    find += 1;
    length = _route.length() - find;
    _contentType = _route.substr(find, length);
    find = -1;
    it = _config.getMimeTypes().begin();

    while (it != _config.getMimeTypes().end())
    {
        if ((it->first).compare(_contentType) == 0)
            break;
        it++;
    }
    if (it != _config.getMimeTypes().end())
        _contentType = it->second;
    else
        _contentType = _config.getType(_location);
}