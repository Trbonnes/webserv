#include "../Methods.hpp"

void        Methods::get()
{
    std::string root;
    int         find;
    int         fd;


            // std::cout << _socket.getRequestURI() << std::endl;

    fd = setRoot();
    if (_statusCode != NOT_FOUND)
    {
        setBody(fd);
        setStat();
        setContentType();
        setContentLength();
        setServerName();
    }
    return ;
}

void        Methods::setServerName()
{
    _server = getServerName(_uri);
}

void        Methods::setContentLength()
{
    _contentLength = _stat.st_size;
}

void        Methods::setStat()
{
    stat(_route.c_str(), &_stat);
}

void        Methods::setContentType()
{
    int                     find;
    int                     length;
    std::string             line;
    std::list<std::string>  mimeTypes;
    std::list<std::string>::iterator it;

    find = _route.find('.', 0);
    find += 1;
    length = _route.length() - find;
    _contentType = _route.substr(find, length);
    find = -1;
    it = getMimeTypes().begin();
    while (it != getMimeTypes().end())
    {
        if ((find = (*it).find(_contentType)) >= 0)
            break ;
        it++;
    }
    if (find >= 0)
        _contentType = (*it).substr(0, (*it).find(" "));
    else
        _contentType = getType(_uri);
}

int         Methods::openFile()
{
    int         fd;
    struct stat file;
    std::string str;
    std::list<std::string>::iterator itIndexBegin;
    std::list<std::string>::iterator itIndexEnd;

    itIndexBegin = getIndex(_location).begin();
    itIndexEnd = getIndex(_location).end();
    stat(_route.c_str(), &file);
    str.assign(_route);
    while (itIndexBegin != itIndexEnd && (file.st_mode & S_IFMT) == S_IFDIR)
    {
        str.assign(_route).append(*itIndexBegin);
        std::cout << str << std::endl;
        stat(str.c_str(), &file);
        itIndexBegin++;
    }
    _route.assign(str);
    fd = open(_route.c_str(), O_RDONLY);
    if (fd < 0)
        _statusCode = NOT_FOUND;
    else
       _statusCode = OK;
    return fd;
}

int         Methods::setRoot()
{
    int         fd;
    int         find;
    std::string str;

    // if (_socket.getRequestURI().compare(0, 2, "/") == 0)
    if (_uri.compare(0, 2, "/") == 0)
    {
        //** Default index page **
        std::list<std::string>::iterator it;

        it = getIndex(_uri).begin();
        while (it != getIndex(_uri).end())
        {
            _route.assign(getRoot(_location));
            _route.append(acceptLanguage());
            _route.append("/");
            _route.append(*it);

            //** Open and test if the file exist **
            if ((fd = openFile()) < 0)
                it++;
            else
                break ;
        }
    }
    else
    {
        // if (_socket.getRequestURI().compare(0, 4, "http") == 0)
        if (_uri.compare(0, 4, "http") == 0)
        {
            //** Absolute path **

            find = _route.append(_socket.getRequestURI()).find(getServerName(_uri));
            _route.erase(0, find + getServerName(_uri).length());
            _route.insert(0, getRoot(_location));
            _route.insert(getRoot(_location).length(), acceptLanguage());
        }
        else
        {
            //** Relative path **

            _route.assign(getRoot(_location));
            _route.append(acceptLanguage());
            _route.append(str.assign(_uri).erase(0, getRoot(_location).length()));
        }
        //** Open and test if the file exist **
        fd = openFile();
    }
    return (fd);
}

//** Copy file into body string **
void        Methods::setBody(int fd)
{
    int     ret;
    char    buf[1024 + 1];

    while ((ret = read(fd, buf, 1024)) > 0)
    {
        buf[ret] = '\0';
        _body.assign(buf);
    }
    if (ret == -1)
        _statusCode = INTERNAL_SERVER_ERROR;
    else
    close(fd);
}

//** Define the language page **
std::string     Methods::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServerBegin;
    std::vector<std::string>::iterator itServerEnd;
    std::vector<std::string>::iterator itServer;
    std::string str;
    std::string trydir;
    struct stat dir;

    str.assign("/");
    itClientBegin = _socket.getAcceptLanguage().begin();
    itClientEnd = _socket.getAcceptLanguage().end();
    itServerEnd = getLanguage(_uri).end();
    while (itClientBegin != itClientEnd)
    {
        itServer = std::find(getLanguage(_uri).begin(), getLanguage(_uri).end(), *itClientBegin);
        if (itServer != itServerEnd)
        {
            _contentLanguage = *itServer;
            str.append(*itServer);
            str.append("/");
            stat(trydir.assign(_route).append(str).c_str(), &dir);
            if ((dir.st_mode & S_IFMT) == S_IFDIR)
                return (str);
            else
                return ("");
        }
        itClientBegin++;
    }
    _contentLanguage = *(getLanguage(_uri).begin());
    str.append(*(getLanguage(_uri).begin()));
    str.append("/");
    stat(trydir.assign(_route).append(str).c_str(), &dir);
    if ((dir.st_mode & S_IFMT) == S_IFDIR)
        return (str);
    else
        return ("");
}
