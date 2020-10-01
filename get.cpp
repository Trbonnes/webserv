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
        setContentType(root);
    }

    std::cout << "ROOT: " << root << std::endl << std::endl;
    std::cout << "STATUS CODE: " << _statusCode << std::endl << std::endl;
    std::cout << "CONTENT TYPE: " << _contentType << std::endl << std::endl;
    std::cout << "BODY: " << std::endl << _body << std::endl << std::endl;

    return ;
}

void        Methods::setContentType(std::string root)
{
    int                     find;
    int                     length;
    std::string             line;
    std::list<std::string>  mimeTypes;
    std::list<std::string>::iterator it;

    find = root.find('.', 0);
    if (find == -1)
        _contentType = "txt";
    find += 1;
    length = root.length() - find;
    _contentType = root.substr(find, length);
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
        _contentType = "text/plain";
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
    close(fd);
}

//** Define the language page **
std::string     Methods::acceptLanguage()
{
    std::vector<std::string>::iterator itClient;
    std::vector<std::string>::iterator itServer;
    std::string str;

    str.assign("/");
    itClient = _socket.getAcceptLanguage().begin();
    while (itClient != _socket.getAcceptLanguage().end())
    {
        itServer = getDefaultLanguage().begin();
        while (itServer != getDefaultLanguage().end() && (*itClient).compare(*itServer) != 0)
            itServer++;
        if (itServer != getDefaultLanguage().end())
            return (str.append(*itServer));
        itClient++;
    }
    return (str.append(*(getDefaultLanguage().begin())));
}
