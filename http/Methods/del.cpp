#include "../HTTP.hpp"

void    HttpResponse::del()
{
    int ret;

    authorization();
    if (_statusCode == OK)
    {
        setStat();
        setDate();
        ret = -1;
        if ((_stat.st_mode & S_IFMT) == S_IFREG)
            ret = unlink(_route.c_str());
        else if (((_stat.st_mode & S_IFMT) == S_IFDIR))
            ret = rmdir(_route.c_str());
        if (ret == -1)
            _statusCode = NO_CONTENT;
        else
            _statusCode = OK;
    }
}
