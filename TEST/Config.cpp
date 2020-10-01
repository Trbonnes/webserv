#include "Config.hpp"

Config::Config() :
_root("/sgoinfre/goinfre/Perso/pganglof/webserver/www"),
_serverName("localhost"),
_index(0),
_defaultType("text/plain") {
    _index.push_back("index.php");
    _index.push_back("index.html");

    //** open mime.types **
    int     ret;
    int     fd;
    char    *line;

    if ((fd = open("../mime.types", O_RDONLY)) >= 0)
    {
        while ((ret = get_next_line(fd, &line)) > 0)
            _mimeTypes.push_back(std::string(line));
        close (fd);
    }
}

Config::Config(Config &copy)
{
    _root = copy._root;
}

Config::~Config() {}

Config                  &Config::operator=(Config const &rhs)
{
    _root = rhs._root;
    return *this;
}

std::string             Config::getRoot()
{
    return _root;
}

std::string             Config::getServerName()
{
    return _serverName;
}

std::list<std::string>  &Config::getIndex()
{
    return _index;
}

std::string             Config::getDefaultType()
{
    return _defaultType;
}

std::list<std::string>  &Config::getMimeTypes()
{
    return _mimeTypes;
}
