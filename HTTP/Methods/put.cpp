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
        struct tm *timeinfo;
        char file[100];
        char modification[100];

        _route = _contentLocation;
        setStat();
        _statusCode = NO_CONTENT;
        timeinfo = localtime(&(_stat.st_ctime));
        strftime(file, 100, "%a %d %b %y %OH %OM %OS", timeinfo);
        write(fd, _socket.getBody().c_str(), _socket.getBody().length());
        setStat();
        timeinfo = localtime(&(_stat.st_ctime));
        strftime(modification, 100, "%a %d %b %y %OH %OM %OS", timeinfo);
        printf("File: %s\n", file);
        printf("Modification: %s\n", modification);
        if (strcmp(file, modification))
            _statusCode = OK;
    }
}
