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

    std::cout << root << std::endl;
    std::cout << _body << std::endl;
    std::cout << _contentType << std::endl;
    std::cout << _statusCode << std::endl;

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
    else
    {
        find += 1;
        length = root.length() - find;
        _contentType = root.substr(find, length);
        // mimeTypes = getMimeTypes();
        it = getMimeTypes().begin();
        while (it != getMimeTypes().end())
        {
            (*it).find(_contentType);
            it++;
        }
        if (it != getMimeTypes().end())
            _contentType = (*it).substr(0, (*it).find(" "));
        else
            _contentType = "text/plain";
    }
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
        }
        else
        {
            //** Relative path **

            root.assign(getRoot());
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
