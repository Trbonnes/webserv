#include "Config.hpp"

Config::Config() :
_root("/home/pauline/webserver/www"),
_serverName("localhost"),
_index(0),
_defaultType("text/plain"),
_configFiles("/home/pauline/webserver/config")
{
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("POST");
    _defaultAllow.push_back("PUT");
    _defaultLanguage.push_back("fr");
    _defaultLanguage.push_back("en");
    _index.push_back("index.html");
    _index.push_back("index.php");

    //** open mime.types **
    int         ret;
    int         fd;
    char        *line;
    std::string string;
    std::string file;
    std::list<std::string>::iterator it;
    std::string::iterator s_it;

    file.append(_configFiles);
    file.append("/mime.types");
    if ((fd = open(file.c_str(), O_RDONLY)) >= 0)
    {
        while ((ret = get_next_line(fd, &line)) > 0)
        {
            string = line;
            _mimeTypes.push_back(string);
        }
        it = _mimeTypes.begin();
        while (it != _mimeTypes.end())
        {
            s_it = (*it).begin();
            while (*s_it == ' ')
                s_it++;
            (*it).erase((*it).begin(), s_it);
            it++;
        }
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

std::list<std::string>      &Config::getMimeTypes()
{
    return _mimeTypes;
}

std::vector<std::string>    &Config::getDefaultLanguage()
{
    return _defaultLanguage;
}

std::vector<std::string>    &Config::getDefaultAllow()
{
    return _defaultAllow;
}
