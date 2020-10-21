#include "../HTTP.hpp"

void        HTTP::put()
{
    int         fd;
    std::string str;

    std::cout << _config.getPutRequestRoot() << std::endl;
    _contentLocation.assign(_config.getPutRequestRoot()).append(_socket.getRequestURI());
    fd = open(_contentLocation.c_str(), O_WRONLY);
    if (fd == -1)
    {
        fd = open(_contentLocation.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
        write(fd, _socket.getBody().c_str(), _socket.getBody().length());
        _statusCode = CREATED;
    }
    else
    {
        _route = _contentLocation;
        setStat();
        _statusCode = NO_CONTENT;
        
    }
}
