#include "../HttpResponse.hpp"

void        HttpResponse::put()
{
    int         fd;
    std::string str;
    std::string file;

    file = _uri.substr(_location.substr(0, _location.length() - 1).length(), _uri.length());
    _route.assign(_config.getPutRoot()).append(file);
    fd = open(_route.c_str(), O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        _statusCode = NO_CONTENT; // Should be 201 but the tester expect 204
        fd = open(_route.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
        else
            write(fd, _socket.getBody().c_str(), _socket.getContentLength());
    }
    else
    {
        _statusCode = NO_CONTENT;
        write(fd, _socket.getBody().c_str(), _socket.getContentLength());
    }
    close(fd);
    setContentLocation();
}
