#include "../HTTP.hpp"

void        HTTP::get()
{
    std::string root;
    int         find;
    int         fd;

    setRoot();
    fd = openFile();
    if (_statusCode == OK)
    {
        authorization();
        if (_statusCode == OK)
        {
            setBody(fd);
            setStat();
            setLastModified();
            setContentType();
            setCharset();
            setContentLength();
            setServerName();
            setContentLocation();
            setDate();
        }
    }
    else if (_config.getAutoindex(_location) == true)
    {
        setAutoindex();
        setContentType();
        setCharset();
        setDate();
        _contentLanguage = "";
        _charset = "";
        // TransferEncoding();
        _statusCode = OK;
    }
    return ;
}

//** Define the language page **
std::string     HTTP::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;

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
                stat(trydir.assign(_route).append(str).c_str(), &dir);
                if ((dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);
                else
                    _contentLanguage.assign("");
            }
            itClientBegin++;
        }
        _contentLanguage = *(_config.getLanguage(_location).begin());
        str.append(*(_config.getLanguage(_uri).begin()));
        stat(trydir.assign(_route).append(str).c_str(), &dir);
        if ((dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
        else
            return (_contentLanguage.assign(""));
    }
    return ("");
}
