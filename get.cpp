#include "Methods.hpp"

void        Methods::get()
{
    std::string root;
    int         find;
    int         fd;


            // std::cout << _socket.getRequestURI() << std::endl;

    //** Set the string root **
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
        if (it == getIndex().end())
            return ; 
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
        if ((fd = openFile(root)) < 0)
            return ;
    }
     
        // std::cout << root << std::endl;

    //** Copy file into body string **
    copyFile(fd);

    return ;
}


int         Methods::openFile(std::string root)
{
    int     fd;

    if ((fd = open(root.c_str(), O_RDONLY)) < 0)
    {
        _statusCode = NOT_FOUND;
        return -1;
    }
    _statusCode = OK;
    return fd;
}

//** Copy file into body string **
void        Methods::copyFile(int fd)
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
    
        std::cout << _statusCode << std::endl;
        std::cout << _body << std::endl;
}
