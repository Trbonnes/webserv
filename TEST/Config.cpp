#include "Config.hpp"

Config::Config() :
_defaultRoot("/home/pauline/webserver/www"),
_defaultServerName("localhost"),
_defaultIndex(0),
_defaultType("text/plain"),
_defaultCharset("koi8-r"),
_defaultLanguage(0),
_configFilesRoot("/home/pauline/webserver/config"),
_autoindexRoot("/home/pauline/webserver/config/autoindex.html"),
_defaultAuth_basic("\"Authorization\""),
_defaultAuth_basic_user_file("/home/pauline/webserver/config/.htpasswd"),
_defaultAutoindex("off")
{
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("POST");
    _defaultAllow.push_back("PUT");
    // _defaultLanguage.push_back("fr");
    // _defaultLanguage.push_back("en");
    _defaultIndex.push_back("index.html");
    // _defaultIndex.push_back("index.php");

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
        string = line;
        _mimeTypes.push_back(string);
        it = _mimeTypes.begin();
        while (it != _mimeTypes.end())
        {
            s_it = (*it).begin();
            while (*s_it == ' ')
                s_it++;
            (*it).erase((*it).begin(), s_it);
            it++;
        }
        free(line);
        close (fd);
    }

    //** Locations **

        //** first location **

    std::list<std::string> index;

    index.push_back("index.php");

    _locationList.push_back(Location("/data/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultServerName, index,
    "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex));

        //** second location **

    std::list<std::string> index2;

    index2.push_back("42.png");

    _locationList.push_back(Location("/images/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultServerName, index2,
    "text/html", "", _defaultLanguage, "off", "", _defaultAutoindex));

    _locationList.push_back(Location("/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultServerName, _defaultIndex,
    _defaultType, _defaultCharset, _defaultLanguage, "off", "", "on"));
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

std::string             Config::getLocation(std::string uri)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (uri.find(itBegin->_location) != std::string::npos)
            return (itBegin->_location);
        itBegin++;
    }
    return (_defaultRoot);
}


std::string             Config::getRoot(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd;
    std::string                     str;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->_location) == 0)
            return (str.assign((itBegin->_root)).append(itBegin->_location));
        itBegin++;
    }
    return _defaultRoot;
}

std::string             Config::getServerName(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_serverName);
        itBegin++;
    }
    return _defaultServerName;
}

std::list<std::string>  &Config::getIndex(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_index);
        itBegin++;
    }
    return _defaultIndex;
}

std::string                 Config::getType(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_type);
        itBegin++;
    }
    return _defaultType;
}

std::list<std::string>      &Config::getMimeTypes()
{
    return _mimeTypes;
}

std::vector<std::string>    &Config::getLanguage(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_language);
        itBegin++;
    }
    return _defaultLanguage;
}

std::vector<std::string>    &Config::getAllow(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_allow);
        itBegin++;
    }
    return _defaultAllow;
}

std::string                 Config::getCharset(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_charset);
        itBegin++;
    }
    return _defaultCharset;   
}

std::string                 Config::getAuth_basic(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_auth_basic);
        itBegin++;
    }
    return _defaultAuth_basic;   
}

std::string                 Config::getAuth_basic_user_file(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_auth_basic_user_file);
        itBegin++;
    }
    return _defaultAuth_basic_user_file;   
}

std::string                 Config::getAutoindex(std::string location)
{
    std::list<Location>::iterator itBegin;
    std::list<Location>::iterator itEnd; 

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.find(itBegin->_location) != std::string::npos)
            return (itBegin->_autoindex);
        itBegin++;
    }
    return _defaultAutoindex;   
}

std::string                 Config::getAutoindexRoot()
{
    return _autoindexRoot;
}