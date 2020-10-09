#include "../Methods.hpp"

void        Methods::get()
{
    std::string root;
    int         find;
    int         fd;

    fd = setRoot();
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
    else if (getAutoindex(_location).compare("on") == 0)
    {
        setAutoindex();
        setContentType();
        setCharset();
        setDate();
        // TransferEncoding();
        _statusCode = OK;
    }
    return ;
}

void        Methods::setServerName()
{
    _server = getServerName();
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

    timeinfo = localtime(&(_stat.st_mtimespec.tv_sec));
    strftime(_lastModified, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
}

void        Methods::setDate()
{
    struct timeval  tv;
    struct tm       *timeinfo;

    if (gettimeofday(&tv, NULL) == 0)
    {
        timeinfo = localtime(&(tv.tv_sec));
        strftime(_date, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
    }
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
        _contentType = getType(_location);
}

int         Methods::openFile()
{
    int         fd;
    struct stat file;
    std::string str;
    std::list<std::string>::iterator itIndexBegin;
    std::list<std::string>::iterator itIndexEnd;

    fd = -1;
    itIndexBegin = getIndex(_location).begin();
    itIndexEnd = getIndex(_location).end();
    stat(_route.c_str(), &file);
    str.assign(_route);
    while (itIndexBegin != itIndexEnd && (file.st_mode & S_IFMT) != S_IFREG)
    {
        str.assign(_route);
        if (str.back() != '/')
            str.append("/");
        str.append(*itIndexBegin);
        stat(str.c_str(), &file);
        itIndexBegin++;
    }
    _route.assign(str);
    if ((file.st_mode & S_IFMT) == S_IFREG)
    {
        fd = open(_route.c_str(), O_RDONLY);
       _statusCode = OK;
    }
    else
        _statusCode = FORBIDDEN;
    return fd;
}

int         Methods::setRoot()
{
    int         fd;
    int         find;
    std::string str;
    struct stat file;

    if (_uri.compare(0, 4, "http") == 0)
    {
        //** Absolute path **

        find = _route.append(_socket.getRequestURI()).find(getServerName());
        
        
        _route.erase(0, find + getServerName().length());
        _route.insert(0, getRoot(_location));
        _route.insert(getRoot(_location).length(), acceptLanguage());
    }
    else
    {
        //** Relative path **

        if (getLocation(_socket.getRequestURI()).compare(_socket.getRequestURI()) != 0)
        {
            _route.assign(getRoot(_location));
            _route.append(_socket.getRequestURI());
            stat(_route.c_str(), &file);
            if ((file.st_mode & S_IFMT) == S_IFREG)
            {
                fd = open(_route.c_str(), O_RDONLY);
                return (fd);
            }
            else
            {
                _statusCode = NOT_FOUND;
                return (-1);
            }
        }
        _route.assign(getRoot(_location));
        _route.append(acceptLanguage());
        _route.append(str.assign(_socket.getRequestURI()).erase(0, _location.length()));
    }
    //** Open and test if the file exist **
    fd = openFile();
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
        // str.assign("/");
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

void            Methods::setAutoindex(void)
{
    std::string     str;
    struct stat     directory;
    DIR             *dir;
    struct dirent   *dirent;
    struct tm       *timeinfo;
    char            lastModifications[100];
    std::stack<std::string> files;

    _body.assign("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
    dir = opendir(_uri.c_str());
    if (dir == NULL)
        _statusCode = INTERNAL_SERVER_ERROR;
    else
    {
        while ((dirent = readdir(dir)) != NULL)
        {
            stat(str.assign(_uri).append(dirent->d_name).c_str(), &directory);
            if (str.assign(dirent->d_name).compare(".") == 0)
                continue ;
            str.assign("<a href=\"");
            str.append(dirent->d_name);
            if (dirent->d_type == DT_DIR)
                str.append("/");
            str.append("\">");
            str.append(dirent->d_name);
            if (dirent->d_type == DT_DIR)
                str.append("/");
            str.append("</a>\t\t\t\t");
            timeinfo = localtime(&(directory.st_mtimespec.tv_sec));
            strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
            str.append(lastModifications);
            str.append("\t\t");
            if (dirent->d_type == DT_DIR)
                str.append("-");
            else
                str.append(std::to_string(directory.st_size));
            str.append("\n");
            files.push(str);
        }
        while (!files.empty())
        {
            _body.append(files.top());
            files.pop();
        }
    }
    _body.append("</pre><hr></body>\n</html>\n");
}

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
                free(line);
            }
        }
    }
    else
        _wwwAuthenticate.assign("OK");
}
