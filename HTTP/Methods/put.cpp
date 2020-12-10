#include "../HTTP.hpp"

void        HTTP::put()
{
    int         fd;
    std::string str;
    
    fd = open("/home/user42/Documents/42Jobs/webserver/put_test/file_should_exit_after", O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        _statusCode = CREATED;
        fd = open("/home/user42/Documents/42Jobs/webserver/put_test/file_should_exit_after", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            _statusCode = INTERNAL_SERVER_ERROR;

        write(fd, _socket.getBody().c_str(), _socket.getBody().length()); // TO DO CONTENT LENGTH
        // write(fd, _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
    }
    else
    {
        _statusCode = NO_CONTENT;
        write(fd, _socket.getBody().c_str(), _socket.getBody().length()); // TO DO CONTENT LENGTH
        // write(fd, _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
    }
    _contentLocation = "http://localhost:8080/home/user42/Documents/42Jobs/webserver/put_test/file_should_exit_after";
    // setContentLocation();
}
