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
        authorization();
        if (_statusCode != UNAUTHORIZED)
        {
            setBody(fd);
            setStat();
            setLastModified();
            setContentType();
            setCharset();
            setContentLength();
            setServerName();
            setContentLocation();
        }
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

void        Methods::setLastModified()
{
    struct tm *timeinfo;

    timeinfo = localtime(&(_stat.st_mtim.tv_sec));
    strftime(_lastModified, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
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
        str.assign(_route);
        if (str.back() != '/')
            str.append("/");
        str.append(*itIndexBegin);
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
            _route.append(str.assign(_socket.getRequestURI()).erase(0, _location.length()));
        }
        //** Open and test if the file exist **
        fd = openFile();
    }
    return (fd);
}

//** Define the language page **
std::string     Methods::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;

    if (!getLanguage(_location).empty())
    {
        str.assign("/");
        itClientBegin = _socket.getAcceptLanguage().begin();
        itClientEnd = _socket.getAcceptLanguage().end();
        itServerEnd = getLanguage(_location).end();
        while (itClientBegin != itClientEnd)
        {
            itServer = std::find(getLanguage(_location).begin(), getLanguage(_location).end(), *itClientBegin);
            if (itServer != itServerEnd)
            {
                _contentLanguage = *itServer;
                str.append(*itServer);
                stat(trydir.assign(_route).append(str).c_str(), &dir);
                if ((dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);
                else
                    return (_contentLanguage.assign(""));
            }
            itClientBegin++;
        }
        _contentLanguage = *(getLanguage(_location).begin());
        str.append(*(getLanguage(_uri).begin()));
        stat(trydir.assign(_route).append(str).c_str(), &dir);
        if ((dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
        else
            return (_contentLanguage.assign(""));
    }
    return ("");
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

void            Methods::authorization()
{
    int     fd;
    int     ret;
    char    *line;

    if (getAuth_basic(_location).compare("off") != 0)
    {
        if (_socket.getAuthorization().compare("") == 0)
        {
            _statusCode = UNAUTHORIZED;
            _wwwAuthenticate.assign("Basic realm=").append(getAuth_basic(_location));
        }
        else
        {
            if ((fd = open(getAuth_basic_user_file(_location).c_str(), O_RDONLY)) >= 0)
            {
                while ((ret = get_next_line(fd, &line)) > 0)
                {
                    if (base64_decode(_socket.getAuthorization()).compare(line) == 0)
                    {
                        _wwwAuthenticate.assign("OK");
                        break ;
                    }
                }
                if (base64_decode(_socket.getAuthorization()).compare(line) == 0)
                    _wwwAuthenticate.assign("OK");
                if (_wwwAuthenticate.compare("OK") != 0)
                {
                    _statusCode = UNAUTHORIZED;
                    _wwwAuthenticate.assign("Basic realm=").append(getAuth_basic(_location));
                }
            }
        }
    }
    else
        _wwwAuthenticate.assign("OK");
}
