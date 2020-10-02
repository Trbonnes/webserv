#include "Config.hpp"

Config::Config() :
_defaultRoot("/home/pauline/webserver/www"),
_defaultServerName("localhost"),
_defaultIndex(0),
_defaultType("text/plain"),
_configFilesRoot("/home/pauline/webserver/config")
{
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("POST");
    _defaultAllow.push_back("PUT");
    _defaultLanguage.push_back("fr");
    _defaultLanguage.push_back("en");
    _defaultIndex.push_back("index.html");
    _defaultIndex.push_back("index.php");

    //** open mime.types **
    int         ret;
    int         fd;
    char        *line;
    std::string string;
    std::string file;
    std::list<std::string>::iterator it;
    std::string::iterator s_it;

    file.append(_configFilesRoot);
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
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultLanguage = copy._defaultLanguage;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;
}

Config::~Config() {}

Config                  &Config::operator=(Config const &rhs)
{
    _defaultRoot = rhs._defaultRoot;
    _defaultAllow = rhs._defaultAllow;
    _defaultServerName = rhs._defaultServerName;
    _defaultIndex = rhs._defaultIndex;
    _defaultType = rhs._defaultType;
    _defaultLanguage = rhs._defaultLanguage;
    _mimeTypes = rhs._mimeTypes;
    _configFilesRoot = rhs._configFilesRoot;
    return *this;
}

std::string             Config::getRoot()
{
    return _defaultRoot;
}

std::string             Config::getServerName()
{
    return _defaultServerName;
}

std::list<std::string>  &Config::getIndex()
{
    return _defaultIndex;
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
