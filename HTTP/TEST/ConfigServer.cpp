#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_defaultRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/www"),
_defaultServerName("localhost"),
_defaultPort("80"),
_httpVersion("HTTP/1.1"),
_serverSoftware("SuperServer/1.0"),
_defaultIndex(0),
_defaultType("text/plain"),
_defaultCharset("utf-8"),
_defaultLanguage(0),
_configFilesRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/config"),
_errorFilesRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/error"),
_defaultAuth_basic("\"Authorization\""),
_defaultAuth_basic_user_file("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/config/.htpasswd"),
_defaultAutoindex(false),
_defaultMaxBody(-1),
_defaultCgi_root("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/bin-cgi/cgi_tester")
{
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("PUT");
    _defaultAllow.push_back("DELETE");
    _defaultLanguage.push_back("en");
    _defaultLanguage.push_back("fr");
    // _defaultIndex.push_back("cgi.bla");
    _defaultIndex.push_back("index.html");
    _defaultIndex.push_back("index.php");

    _defaultCgi.push_back("bla");
    _defaultCgi_methods.push_back("POST");

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
            free(line);
            line = NULL;
            _mimeTypes.push_back(string);
        }
        string = line;
        free(line);
        line = NULL;
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
        close (fd);
    }

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

    Location loc1("/data/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www",
    _defaultAllow, index,
    "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex, "/blabla/", exe, cgi_methods, "",
    _defaultMaxBody);

    _locationList["/data/"] = loc1;

        //** second location **
    std::vector<std::string> index2;
    std::vector<std::string> exe1;

    index2.push_back("42.png");

    Location loc2("/images/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www",
    _defaultAllow, index2,
    "text/html", "", _defaultLanguage, "off", "", _defaultAutoindex, "", exe1, cgi_methods, "",
    _defaultMaxBody);

    _locationList["/images/"] = loc2;

        //** third location **
    std::vector<std::string> exe2;

    Location loc3("/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www",
    _defaultAllow, _defaultIndex,
    _defaultType, _defaultCharset, _defaultLanguage, "off", "", true, "", _defaultCgi, cgi_methods, _defaultCgi_root,
    -1);

    _locationList["/"] = loc3;

        //** fourth location **
    std::vector<std::string> exe3;

    // exe3.push_back("bla");
    exe3.push_back("php");

    Location loc4("/cgi/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www", _defaultAllow, _defaultIndex, _defaultType, 
    _defaultCharset, _defaultLanguage, "off", "", false, "", exe3, cgi_methods, "/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/bin-cgi/cgi_tester",
    _defaultMaxBody);

    _locationList["/cgi/"] = loc4;

}


// ConfigServer::ConfigServer() :
// _defaultRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/www"),
// _defaultServerName("localhost"),
// _defaultPort("80"),
// _httpVersion("HTTP/1.1"),
// _serverSoftware("SuperServer/1.0"),
// _defaultIndex(0),
// _defaultType("text/plain"),
// _defaultCharset("utf-8"),
// _defaultLanguage(0),
// _configFilesRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/config"),
// _errorFilesRoot("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/error"),
// _defaultAuth_basic("\"Authorization\""),
// _defaultAuth_basic_user_file("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/config/.htpasswd"),
// _defaultAutoindex(false),
// _defaultMaxBody(-1),
// _defaultCgi_root("/sgoinfre/goinfre/Perso/pganglof/webserv/HTTP/bin-cgi/cgi_tester")
// {
//     _defaultAllow.push_back("GET");
//     _defaultAllow.push_back("HEAD");
//     _defaultLanguage.push_back("en");
//     _defaultLanguage.push_back("fr");
//     // _defaultIndex.push_back("cgi.bla");
//     _defaultIndex.push_back("index.html");
//     _defaultIndex.push_back("index.php");

//     _defaultCgi.push_back("bla");
//     _defaultCgi_methods.push_back("POST");

//     //** open mime.types **
//     int         ret;
//     int         fd;
//     char        *line;
//     std::string string;
//     std::string file;
//     std::list<std::string>::iterator it;
//     std::string::iterator   s_it;

//     file.append(_configFilesRoot);
//     file.append("/mime.types");
//     if ((fd = open(file.c_str(), O_RDONLY)) >= 0)
//     {
//         while ((ret = get_next_line(fd, &line)) > 0)
//         {
//             string = line;
//             free(line);
//             line = NULL;
//             _mimeTypes.push_back(string);
//         }
//         string = line;
//         free(line);
//         line = NULL;
//         _mimeTypes.push_back(string);
//         it = _mimeTypes.begin();
//         while (it != _mimeTypes.end())
//         {
//             s_it = (*it).begin();
//             while (*s_it == ' ')
//                 s_it++;
//             (*it).erase((*it).begin(), s_it);
//             it++;
//         }
//         close (fd);
//     }

//     //** Locations **

//         //** First location **
//     std::vector<std::string> no_exe;
//     std::vector<std::string> allow1;

//     allow1.push_back("GET");

//     Location loc1("/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www",
//     allow1, _defaultIndex,
//     _defaultType, _defaultCharset, _defaultLanguage, "off", "", true, "", _defaultCgi, _defaultCgi_methods, _defaultCgi_root,
//     _defaultMaxBody);

//     _locationList["/"] = loc1;

//         //** Second location **
//     std::vector<std::string> allow2;

//     allow2.push_back("PUT");

//     Location loc2("/put_test/*", "/sgoinfre/goinfre/Perso/pganglof/webserv/www", allow2, _defaultIndex, _defaultType, 
//     _defaultCharset, _defaultLanguage, "off", "", false, "", _defaultCgi, _defaultCgi_methods, _defaultCgi_root,
//     _defaultMaxBody);

//     _locationList["/put_test/"] = loc2;

//     //** Third location **
//     std::vector<std::string> allow3;

//     allow3.push_back("POST");

//     Location loc3("/post_body", "/sgoinfre/goinfre/Perso/pganglof/webserv/www", allow3, _defaultIndex, _defaultType, 
//     _defaultCharset, _defaultLanguage, "off", "", false, "", _defaultCgi, _defaultCgi_methods, _defaultCgi_root, 100);

//     _locationList["/post_body"] = loc3;

//    //** Fourth location **
//     std::vector<std::string> allow4;
//     std::vector<std::string> index;
    
//     allow4.push_back("GET");
//     index.push_back("youpi.bad_extension");

//     Location loc4("/directory/", "/sgoinfre/goinfre/Perso/pganglof/webserv/www", allow4, index, _defaultType, 
//     _defaultCharset, _defaultLanguage, "off", "", false, "", _defaultCgi, _defaultCgi_methods, _defaultCgi_root, 
//     _defaultMaxBody);

//     _locationList["/directory/"] = loc4;

// }

ConfigServer::ConfigServer(ConfigServer &copy)
{
    _locationList = copy._locationList;
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultPort = copy._defaultPort;
    _httpVersion = copy._httpVersion;
    _serverSoftware = copy._serverSoftware;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultCharset = copy._defaultCharset;
    _defaultLanguage = copy._defaultLanguage;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;
    _errorFilesRoot = copy._errorFilesRoot;
    _defaultAuth_basic = copy._defaultAuth_basic;
    _defaultAuth_basic_user_file = copy._defaultAuth_basic_user_file;
    _defaultAutoindex = copy._defaultAutoindex;
    _defaultCgi = copy._defaultCgi;
    _defaultCgi_methods = copy._defaultCgi_methods;
    _defaultCgi_root = copy._defaultCgi_root;
    _defaultMaxBody = copy._defaultMaxBody;
}

ConfigServer::~ConfigServer() {}

ConfigServer                  &ConfigServer::operator=(ConfigServer const &rhs)
{
    _locationList = rhs._locationList;
    _defaultRoot = rhs._defaultRoot;
    _defaultAllow = rhs._defaultAllow;
    _defaultServerName = rhs._defaultServerName;
    _defaultPort = rhs._defaultPort;
    _httpVersion = rhs._httpVersion;
    _serverSoftware = rhs._serverSoftware;
    _defaultIndex = rhs._defaultIndex;
    _defaultType = rhs._defaultType;
    _defaultCharset = rhs._defaultCharset;
    _defaultLanguage = rhs._defaultLanguage;
    _mimeTypes = rhs._mimeTypes;
    _configFilesRoot = rhs._configFilesRoot;
    _errorFilesRoot = rhs._errorFilesRoot;
    _defaultAuth_basic = rhs._defaultAuth_basic;
    _defaultAuth_basic_user_file = rhs._defaultAuth_basic_user_file;
    _defaultAutoindex = rhs._defaultAutoindex;
    _defaultCgi = rhs._defaultCgi;
    _defaultCgi_methods = rhs._defaultCgi_methods;
    _defaultCgi_root = rhs._defaultCgi_root;
    _defaultMaxBody = rhs._defaultMaxBody;
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
            return (str.assign((itBegin->second._root)).append(itBegin->second._location.substr(1)));
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
    return _defaultPort;
}

std::string             ConfigServer::getHttpVersion()
{
    return _httpVersion;
}

std::string             ConfigServer::getServerSoftware()
{
    return _serverSoftware;
}

std::string             ConfigServer::getErrorFilesRoot()
{
    return _errorFilesRoot;
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
    return _defaultCgi_root;
}

int                     ConfigServer::getClientBodySize(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._maxBody);
        itBegin++;
    }
    return _defaultMaxBody;
}
