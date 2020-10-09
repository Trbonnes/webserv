/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:13:35 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 16:53:45 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_defaultRoot("/home/pauline/webserver/www"),
_defaultServerName("localhost"),
_defaultIndex(0),
_defaultType("text/plain"),
_defaultCharset("koi8-r"),
_defaultLanguage(0),
_configFilesRoot("/home/pauline/webserver/config"),
_defaultAuth_basic("\"Authorization\""),
_defaultAuth_basic_user_file("/home/pauline/webserver/config/.htpasswd"),
_defaultAutoindex("off") {
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("POST");
    _defaultAllow.push_back("PUT");
    _defaultLanguage.push_back("fr");
    _defaultLanguage.push_back("en");
    _defaultIndex.push_back("index.html");
    _defaultIndex.push_back("index.html");
    _mimeTypes.push_back("plain/text");

/*

    -----mime.types test not for configfile parsing-----

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
        while ((ret = getDefault_next_line(fd, &line)) > 0)
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
*/

/*

    -----Test Locations without config file parsing-----

    //** Locations **

        //** first location **

    std::list<std::string> index;
    
    index.push_back("index.php");

    Location loc1("/data/", "/home/pauline/webserver/www",
    _defaultAllow, index,
    "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex);

    _locationList["/data/"] = loc1;

        //** second location **

    std::list<std::string> index2;

    index2.push_back("42.png");

    Location loc2("/images/", "/home/pauline/webserver/www",
    _defaultAllow, index2,
    "text/html", "", _defaultLanguage, "off", "", _defaultAutoindex);

    _locationList["/images/"] = loc2;

    Location loc3("/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultIndex,
    _defaultType, _defaultCharset, _defaultLanguage, "off", "", "on");

    _locationList["/"] = loc3;
*/

}

ConfigServer::ConfigServer(const ConfigServer &copy)
{
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultLanguage = copy._defaultLanguage;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;

    _cgi = copy._cgi;
    _cgi_method = copy._cgi_method;
    _cgi_root = copy._cgi_root;

    _port = copy._port;
    _server_name = copy._server_name;

    _root = copy._root;
    _index = copy._index;
    _autoIndex = copy._autoIndex;
    _clientBodySize = copy._clientBodySize;
    _acceptedMethods = copy._acceptedMethods;
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

    _cgi = rhs._cgi;
    _cgi_method = rhs._cgi_method;
    _cgi_root = rhs._cgi_root;

    _port = rhs._port;
    _server_name = rhs._server_name;

    _root = rhs._root;
    _index = rhs._index;
    _autoIndex = rhs._autoIndex;
    _clientBodySize = rhs._clientBodySize;
    _acceptedMethods = rhs._acceptedMethods;
    return *this;
}

std::string             ConfigServer::getDefaultLocation(std::string uri)
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

std::string             ConfigServer::getDefaultRoot(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;    
    std::string str;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (str.assign((itBegin->second.getRoot())).append(itBegin->second.getLocation()));
        itBegin++;
    }
    return _defaultRoot;
}

std::string             ConfigServer::getDefaultServerName(std::string location)
{
    return _defaultServerName;
}

std::vector<std::string>  &ConfigServer::getDefaultIndex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;    

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getIndex());
        itBegin++;
    }
    return _defaultIndex;
}

std::string                 ConfigServer::getDefaultType(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getType());
        itBegin++;
    }
    return _defaultType;
}

std::list<std::string>      &ConfigServer::getDefaultMimeTypes()
{
    return _mimeTypes;
}

std::vector<std::string>    &ConfigServer::getDefaultLanguage(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getLanguage());
        itBegin++;
    }
    return _defaultLanguage;
}

std::vector<std::string>    &ConfigServer::getDefaultAllow(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getAllow());
        itBegin++;
    }
    return _defaultAllow;
}

std::string                 ConfigServer::getDefaultCharset(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getCharset());
        itBegin++;
    }
    return _defaultCharset;   
}

std::string                 ConfigServer::getDefaultAuth_basic(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getAuth_basic());
        itBegin++;
    }
    return _defaultAuth_basic;   
}

std::string                 ConfigServer::getDefaultAuth_basic_user_file(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getAuth_basic_user_file());
        itBegin++;
    }
    return _defaultAuth_basic_user_file;   
}

bool                 ConfigServer::getDefaultAutoindex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second.getAutoIndex());
        itBegin++;
    }
    return _defaultAutoindex;   
}


std::string					ConfigServer::getCgi() {
    return _cgi;
}

std::vector<std::string>			ConfigServer::getCgi_method() {
    return _cgi_method;
}

std::string					ConfigServer::getCgi_root() {
    return _cgi_root;
}


int                         ConfigServer::getPort() {
    return _port;
}

std::vector<std::string>    ConfigServer::getServer_name() {
    return _server_name;
}  


std::string                 ConfigServer::getRoot() {
    return _root;
}

std::vector<std::string>    ConfigServer::getIndex() {
    return _index;
}

bool                        ConfigServer::getAutoIndex() {
    return _autoIndex;
}

int                         ConfigServer::getClientBodySize() {
    return _clientBodySize;
}

std::vector<std::string>    ConfigServer::getAcceptedMethods() {
    return _acceptedMethods;
}

void					ConfigServer::setCgi(std::string cgi) {
    _cgi = cgi;
} 

void					ConfigServer::setCgi_method(std::vector<std::string> cgi_method) {
    _cgi_method = cgi_method;
}

void					ConfigServer::setCgi_root(std::string cgi_root) {
    _cgi_root = cgi_root;
}

void                    ConfigServer::setPort(int port) {
    _port = port;
}

void                    ConfigServer::setServer_name(std::vector<std::string> server_name) {
    _server_name = server_name;
}

void                    ConfigServer::setRoot(std::string root) {
    _root = root;
}

void                    ConfigServer::setIndex(std::vector<std::string> index) {
    _index = index;
}

bool                    ConfigServer::setAutoIndex(bool autoIndex) {
    _autoIndex = autoIndex;
}

void                    ConfigServer::setClientBodySize(int clientBodySize) {
    _clientBodySize = clientBodySize;
}

void                    ConfigServer::setAcceptedMethods(std::vector<std::string> acceptedMethods) {
    _acceptedMethods = acceptedMethods;
}

