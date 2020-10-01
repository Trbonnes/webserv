#include "Methods.hpp"

void        Methods::get()
{
    std::string root;
    int         find;
    int         fd;


            // std::cout << _socket.getRequestURI() << std::endl;

    //** Set the string root **

    fd = setRoot(root);
    if (_statusCode != NOT_FOUND)
    {
        setBody(fd);
        setStat(root);
        setContentType(root);
        setContentLength();
        setServerName();
    }
    std::cout << "ROOT: " << root << std::endl << std::endl;

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

void        Methods::setStat(std::string root)
{
    stat(root.c_str(), &_stat);
}

void        Methods::setContentType(std::string root)
{
    int                     find;
    int                     length;
    std::string             line;
    std::list<std::string>  mimeTypes;
    std::list<std::string>::iterator it;

    find = root.find('.', 0);
    find += 1;
    length = root.length() - find;
    _contentType = root.substr(find, length);
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
        _contentType = getDefaultType();
}

int         Methods::openFile(std::string root)
{
    int     fd;
    int     find;
    int     length;

    if ((fd = open(root.c_str(), O_RDONLY)) < 0)
    {
        _statusCode = NOT_FOUND;
        return -1;
    }
    _statusCode = OK;
    return fd;
}

int         Methods::setRoot(std::string &root)
{
    int     fd;
    int     find;

    if (_socket.getRequestURI().compare(0, 2, "/") == 0)
    {
        //** Default index page **
        std::list<std::string>::iterator it;

        it = getIndex().begin();
        while (it != getIndex().end())
        {
            root.assign(getRoot());
            root.append(acceptLanguage());
            root.append("/");
            root.append(*it);

            //** Open and test if the file exist **
            if ((fd = openFile(root)) < 0)
                it++;
            else
                break ;
        }
    }
    else
    {
        if (_socket.getRequestURI().compare(0, 4, "http") == 0)
        {
            //** Absolute path **

            find = root.append(_socket.getRequestURI()).find(getServerName());
            root.erase(0, find + getServerName().length());
            root.insert(0, getRoot());
            root.insert(getRoot().length(), acceptLanguage());
        }
        else
        {
            //** Relative path **

            root.assign(getRoot());
            root.append(acceptLanguage());
            root.append(_socket.getRequestURI());
        }
        //** Open and test if the file exist **
        fd = openFile(root);
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

    str.assign("/");
    itClientBegin = _socket.getAcceptLanguage().begin();
    itClientEnd = _socket.getAcceptLanguage().end();
    itServerEnd = getDefaultLanguage().end();
    while (itClientBegin != itClientEnd)
    {
        itServer = std::find(getDefaultLanguage().begin(), getDefaultLanguage().end(), *itClientBegin);
        if (itServer != itServerEnd)
        {
            _contentLanguage = *itServer;
            return (str.append(*itServer));
        }
        itClientBegin++;
    }
    _contentLanguage = *(getDefaultLanguage().begin());
    return (str.append(*(getDefaultLanguage().begin())));
}
