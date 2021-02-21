#include "../HttpResponse.hpp"

void        HttpResponse::put()
{
    int         fd = 0;
    std::string str;
    std::string file;

    
    if (fd == -1)
    {
        _statusCode = NO_CONTENT; // Should be 201 but the tester expect 204
        fd = open(_route.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
        else
            write(fd, _request->getBody().c_str(), _request->getContentLength());
    }
    else
    {
        _statusCode = NO_CONTENT;
        write(fd, _request->getBody().c_str(), _request->getContentLength());
    }
    close(fd);
    setContentLocation();
}
