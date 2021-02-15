#include "../HttpResponse.hpp"

void        HttpResponse::get()
{
    std::string root;
    int         fd;

    fd = openFile();
    authorization();
    if (_statusCode == OK)
    {
        setBody(fd);
        setLastModified();
        setContentType();
        setCharset();
        setContentLength();
        setServerName();
        setContentLocation();
        setDate();
    }
    else if (_statusCode != UNAUTHORIZED && _config.getAutoindex(_location) == true)
    {
        setAutoindex();
        setDate();
        _contentLanguage = "";
        _statusCode = OK;
    }
    return ;
}

//** Define the language page **
std::string     HttpResponse::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;
    int r;

    if (!_config.getLanguage(_location).empty())
    {
        str.assign("/");
        itClientBegin = _socket.getAcceptLanguage().begin();
        itClientEnd = _socket.getAcceptLanguage().end();
        itServerEnd = _config.getLanguage(_location).end();
        while (itClientBegin != itClientEnd)
        {
            itServer = std::find(_config.getLanguage(_location).begin(), _config.getLanguage(_location).end(), *itClientBegin);
            if (itServer != itServerEnd)
            {
                _contentLanguage = *itServer;
                str.assign(*itServer);
                str.append("/");
                r = stat(trydir.assign(_route).append(str).c_str(), &dir);
                if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);
                else
                    _contentLanguage.assign("");
            }
            itClientBegin++;
        }
        _contentLanguage = *(_config.getLanguage(_location).begin());
        str.append(*(_config.getLanguage(_uri).begin()));
        r = stat(trydir.assign(_route).append(str).c_str(), &dir);
        if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
        else
            return (_contentLanguage.assign(""));
    }
    return ("");
}
