#include "../HTTP.hpp"

void        HTTP::put()
{
    int         fd;
    std::string str;
    
    fd = open(_route.c_str(), O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        _statusCode = CREATED;
        fd = open(_route.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
        write(fd, _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
    }
    else
    {
        _statusCode = NO_CONTENT;
        write(fd, _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
    }
    setContentLocation();
}
