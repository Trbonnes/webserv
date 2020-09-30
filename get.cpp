#include "Methods.hpp"

void        Methods::get()
{
    std::string root;
    Config      config;
    int         find;

    // Set the string root 
    if (_socket.getRequestURI().compare(0, 4, "http") != 0)
    {
        // Relative path
        root.assign(config.getRoot());
        root.append(_socket.getRequestURI());
    }
    else
    {
        // Absolute path 
        find = root.append(_socket.getRequestURI()).find(config.getServerName());
        root.erase(0, find + config.getServerName().length());
        root.insert(0, config.getRoot());
    }
     
    // std::cout << root << std::endl;

    // Open and copy file into body string
    openFile(root);

    return ;
}

void        Methods::openFile(std::string root)
{
    int     fd;
    int     ret;
    char    buf[1024 + 1];


    if ((fd = open(root.c_str(), O_RDONLY)) < 0)
        _error = BAD_REQUEST;
    else
    {
        while ((ret = read(fd, buf, 1024)) > 0)
        {
            buf[ret] = '\0';
            _body.assign(buf);
            close(fd);
        }
    }

    //   std::cout << _body << std::endl;
}
