#include "Config.hpp"

Config::Config(std::map<std::string, Location, CompLength<std::string> > locationList, std::string defaultRoot, std::vector<std::string> defaultAllow, std::string serverName,
int port, std::list<std::string> defaultIndex, std::string defaultType, std::string defaultCharset, std::vector<std::string> defaultLanguage,
std::list<std::string> mimeTypes, std::string configFilesRoot, std::string defaultAuth_basic, std::string defaultAuth_basic_user_file,
std::string defaultAutoindex) :
_locationList(locationList),
_defaultRoot(defaultRoot),
_defaultAllow(defaultAllow),
_serverName(serverName),
_port(port),
_defaultIndex(defaultIndex),
_defaultType(defaultType),
_defaultCharset(defaultCharset),
_defaultLanguage(defaultLanguage),
_mimeTypes(mimeTypes),
_configFilesRoot(configFilesRoot),
_defaultAuth_basic(defaultAuth_basic),
_defaultAuth_basic_user_file(defaultAuth_basic_user_file),
_defaultAutoindex(defaultAutoindex) {}

Config::Config() :
_locationList(),
_defaultRoot(""),
_defaultAllow(0),
_serverName(""),
_port(0),
_defaultIndex(0),
_defaultType(""),
_defaultCharset(""),
_defaultLanguage(0),
_mimeTypes(0),
_configFilesRoot(""),
_defaultAuth_basic(""),
_defaultAuth_basic_user_file(""),
_defaultAutoindex("")
{
    // _defaultAllow.push_back("GET");
    // _defaultAllow.push_back("HEAD");
    // _defaultAllow.push_back("POST");
    // _defaultAllow.push_back("PUT");
    // _defaultLanguage.push_back("fr");
    // // _defaultLanguage.push_back("en");
    // _defaultIndex.push_back("index.html");
    // // _defaultIndex.push_back("index.php");

    // //** open mime.types **
    // int         ret;
    // int         fd;
    // char        *line;
    // std::string string;
    // std::string file;
    // std::list<std::string>::iterator it;
    // std::string::iterator s_it;

    // file.append(_configFilesRoot);
    // file.append("/mime.types");
    // if ((fd = open(file.c_str(), O_RDONLY)) >= 0)
    // {
    //     while ((ret = get_next_line(fd, &line)) > 0)
    //     {
    //         string = line;
    //         _mimeTypes.push_back(string);
    //     }
    //     string = line;
    //     _mimeTypes.push_back(string);
    //     it = _mimeTypes.begin();
    //     while (it != _mimeTypes.end())
    //     {
    //         s_it = (*it).begin();
    //         while (*s_it == ' ')
    //             s_it++;
    //         (*it).erase((*it).begin(), s_it);
    //         it++;
    //     }
    //     free(line);
    //     close (fd);
    // }

    // //** Locations **

    //     //** first location **

    // std::list<std::string> index;
    
    // index.push_back("index.php");

    // Location loc1("/data/", "/home/pauline/webserver/www",
    // _defaultAllow, index,
    // "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex);

    // _locationList["/data/"] = loc1;

    //     //** second location **

    // std::list<std::string> index2;

    // index2.push_back("42.png");

    // Location loc2("/images/", "/home/pauline/webserver/www",
    // _defaultAllow, index2,
    // "text/html", "", _defaultLanguage, "off", "", _defaultAutoindex);

    // _locationList["/images/"] = loc2;

    // Location loc3("/", "/home/pauline/webserver/www",
    // _defaultAllow, _defaultIndex,
    // _defaultType, _defaultCharset, _defaultLanguage, "off", "", "on");

    // _locationList["/"] = loc3;

}

Config::Config(Config const &copy)
{
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _serverName = copy._serverName;
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
    _serverName = rhs._serverName;
    _defaultIndex = rhs._defaultIndex;
    _defaultType = rhs._defaultType;
    _defaultLanguage = rhs._defaultLanguage;
    _mimeTypes = rhs._mimeTypes;
    _configFilesRoot = rhs._configFilesRoot;
    return *this;
}

std::string             Config::getLocation(std::string uri)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;

    itBegin = _locationList.begin();
        std::cout << "TEST: " << itBegin->first << std::endl;
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (uri.find(itBegin->first) != std::string::npos)
            return (itBegin->first);
        itBegin++;
    }
    return (_defaultRoot);
}

std::string             Config::getRoot(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;    
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

std::string             Config::getServerName(std::string location)
{
    return _serverName;
}

std::list<std::string>  &Config::getIndex(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;    

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

std::string                 Config::getType(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::list<std::string>      &Config::getMimeTypes()
{
    return _mimeTypes;
}

std::vector<std::string>    &Config::getLanguage(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::vector<std::string>    &Config::getAllow(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::string                 Config::getCharset(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::string                 Config::getAuth_basic(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::string                 Config::getAuth_basic_user_file(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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

std::string                 Config::getAutoindex(std::string location)
{
    std::map<std::string, Location, CompLength<std::string> >::iterator itBegin;
    std::map<std::string, Location, CompLength<std::string> >::iterator itEnd;  

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
