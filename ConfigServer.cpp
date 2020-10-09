#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_defaultRoot("/Users/trbonnes/Documents/Tristan42/webserver"),
_defaultServerName("localhost"),
_defaultIndex(0),
_defaultType("text/plain"),
_defaultCharset("koi8-r"),
_defaultLanguage(0),
_configFilesRoot("/Users/trbonnes/Documents/Tristan42/webserver/config"),
_defaultAuth_basic("\"Authorization\""),
_defaultAuth_basic_user_file("/Users/trbonnes/Documents/Tristan42/webserver/config/.htpasswd"),
_defaultAutoindex("off") {
}

ConfigServer::ConfigServer(const ConfigServer &copy) {
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultLanguage = copy._defaultLanguage;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;
}

ConfigServer::~ConfigServer() {}

ConfigServer                  &ConfigServer::operator=(ConfigServer const &rhs)
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

std::string             ConfigServer::getLocation(std::string uri)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (uri.find(itBegin->first) != std::string::npos)
            return (itBegin->first);
        itBegin++;
    }
    return (_defaultRoot);
}

std::string             ConfigServer::getRoot(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;    
    std::string str;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (str.assign((itBegin->second._root)).append(itBegin->second._location));
        itBegin++;
    }
    return _defaultRoot;
}

std::string             ConfigServer::getServerName(std::string location)
{
    return _defaultServerName;
}

std::list<std::string>  &ConfigServer::getIndex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;    

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._index);
        itBegin++;
    }
    return _defaultIndex;
}

std::string                 ConfigServer::getType(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._type);
        itBegin++;
    }
    return _defaultType;
}

std::list<std::string>      &ConfigServer::getMimeTypes()
{
    return _mimeTypes;
}

std::vector<std::string>    &ConfigServer::getLanguage(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._language);
        itBegin++;
    }
    return _defaultLanguage;
}

std::vector<std::string>    &ConfigServer::getAllow(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._allow);
        itBegin++;
    }
    return _defaultAllow;
}

std::string                 ConfigServer::getCharset(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._charset);
        itBegin++;
    }
    return _defaultCharset;   
}

std::string                 ConfigServer::getAuth_basic(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._auth_basic);
        itBegin++;
    }
    return _defaultAuth_basic;   
}

std::string                 ConfigServer::getAuth_basic_user_file(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._auth_basic_user_file);
        itBegin++;
    }
    return _defaultAuth_basic_user_file;   
}

std::string                 ConfigServer::getAutoindex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._autoindex);
        itBegin++;
    }
    return _defaultAutoindex;   
}

