#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_defaultRoot("/home/pauline/webserver/www"),
_defaultServerName("localhost"),
_defaultPort("8080"),
_defaultIndex(0),
_defaultType("text/plain"),
_defaultCharset("koi8-r"),
_defaultLanguage(0),
_configFilesRoot("/home/pauline/webserver/config"),
_defaultAuth_basic("\"Authorization\""),
_defaultAuth_basic_user_file("/home/pauline/webserver/config/.htpasswd"),
_defaultAutoindex(false),
_defaultCgi(0),
_defaultCgi_methods(0)
{
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultLanguage.push_back("en");
    _defaultLanguage.push_back("fr");
    _defaultIndex.push_back("cgi.bla");
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

    std::cout << "Default port: " << _defaultPort << std::endl;

    //** Locations **

        //** first location **

    std::vector<std::string> index;
    std::vector<std::string> exe;
    std::vector<std::string> cgi_methods;

    cgi_methods.push_back("GET");
    cgi_methods.push_back("HEAD");
    cgi_methods.push_back("PUT");
    cgi_methods.push_back("POST");
    cgi_methods.push_back("DELETE");
    index.push_back("index.php");

    Location loc1("/data/", "/home/pauline/webserver/www",
    _defaultAllow, index,
    "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex, "/blabla/", exe, cgi_methods, "");

    _locationList["/data/"] = loc1;

        //** second location **

    std::vector<std::string> index2;
    std::vector<std::string> exe1;

    index2.push_back("42.png");

    Location loc2("/images/", "/home/pauline/webserver/www",
    _defaultAllow, index2,
    "text/html", "", _defaultLanguage, "off", "", _defaultAutoindex, "", exe1, cgi_methods, "");

    _locationList["/images/"] = loc2;

        //** third location **
    std::vector<std::string> exe2;

    Location loc3("/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultIndex,
    _defaultType, _defaultCharset, _defaultLanguage, "off", "", true, "", exe2, cgi_methods, "");

    _locationList["/"] = loc3;

        //** fourth location **
    std::vector<std::string> exe3;

    exe3.push_back("bla");

    Location loc4("/cgi/", "/home/pauline/webserver/www", _defaultAllow, _defaultIndex, _defaultType, 
    _defaultCharset, _defaultLanguage, "off", "", false, "", exe3, cgi_methods, "/home/pauline/webserver/bin-cgi/ubuntu_cgi_tester");

    _locationList["/cgi/"] = loc4;

}

ConfigServer::ConfigServer(ConfigServer &copy)
{
    _locationList = copy._locationList;
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
    _locationList = rhs._locationList;
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

std::string             ConfigServer::getServerName()
{
    return _defaultServerName;
}

std::string             ConfigServer::getPort()
{
    std::cout << "GET PORT: " << _defaultPort << std::endl;
    return _defaultPort;
}

std::vector<std::string>  &ConfigServer::getIndex(std::string location)
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

std::vector<std::string>    &ConfigServer::getCGImethods(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._cgi_methods);
        itBegin++;
    }
    return _defaultCgi_methods;
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

bool                        ConfigServer::getAutoindex(std::string location)
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

std::string             ConfigServer::getAlias(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._alias);
        itBegin++;
    }
    return "";   
}

std::vector<std::string>             &ConfigServer::getCGI(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  
    std::vector<std::string> ret;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._cgi);
        itBegin++;
    }
    return _defaultCgi;   
}

std::string             ConfigServer::getCGI_root(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._cgi_root);
        itBegin++;
    }
    return "";   
}
