/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:13:35 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/15 16:27:38 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_httpVersion("http/1.1"),
_serverSoftware("Server/2.0"),
_putRoot("data/"),
_defaultClientBodySize(-1),
_defaultAutoindex(-1)
{

    // open mime.types **
    int         ret;
    int         fd;
    char        *line;
    std::string string;
    std::string file;
    std::string it;
    std::string::iterator s_it;
    // std::map<std::string, std::string> map;
    size_t space;
    size_t coma; 
    std::string key;
    std::string value;

    file.append("config/mime.types");
    if ((fd = open(file.c_str(), O_RDONLY)) >= 0)
    {
        while ((ret = get_next_line(fd, &line)) > 0)
        {
            it = line;
            free(line);
            
            s_it = it.begin();
            while (*s_it == ' ')
                s_it++;
            it.erase(it.begin(), s_it); // erase espace du debut

            space = it.find(' '); // trouve premier espace
            value = it.substr(0, space); // trouve value
            if (space != std::string::npos)
                it.erase(it.begin(), it.begin() + space); // efface value
            while (1)
            {
                s_it = it.begin(); // retourne au debut
                while (*s_it == ' ') // 
                    s_it++;
                it.erase(it.begin(), s_it); // erase espace du debut

                s_it = it.begin(); // retourne au debut
                space = it.find(' '); // trouve prochain space
                coma = it.find(';');
                if (space != std::string::npos)
                {
                    key = it.substr(0, space);
                    it.erase(it.begin(), s_it + space); // efface value
                    _mimeTypes[key] = value;
                }
                else if (coma != std::string::npos)
                {
                    coma = it.find(';');
                    key = it.substr(0, coma);
                    _mimeTypes[key] = value;
                    break;
                }
                else
                    break;
            }
        }
        free(line);
        close (fd);

    }
}

ConfigServer::ConfigServer(const ConfigServer &copy)
{
    _httpVersion = copy._httpVersion;
    _serverSoftware = copy._serverSoftware;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;
    _errorFilesRoot = copy._errorFilesRoot;
    _putRoot = copy._putRoot;
    _port = copy._port;
    _defaultClientBodySize = copy._defaultClientBodySize;
    _errorPages = copy._errorPages;
	_locationList = copy._locationList;
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultCharset = copy._defaultCharset;
    _defaultLanguage = copy._defaultLanguage;
    _defaultAuth_basic = copy._defaultAuth_basic;
    _defaultAuth_basic_user_file = copy._defaultAuth_basic_user_file;
    _defaultAutoindex = copy._defaultAutoindex;
	_defaultCgi = copy._defaultCgi;
	_defaultCgi_allow = copy._defaultCgi_allow;
	_defaultCgi_root = copy._defaultCgi_root;
}

ConfigServer::~ConfigServer() {
}

ConfigServer                  &ConfigServer::operator=(ConfigServer const &rhs)
{
    _httpVersion = rhs._httpVersion;
    _serverSoftware = rhs._serverSoftware;
    _mimeTypes = rhs._mimeTypes;
    _configFilesRoot = rhs._configFilesRoot;
    _errorFilesRoot = rhs._errorFilesRoot;
    _putRoot = rhs._putRoot;
    _port = rhs._port;
    _defaultClientBodySize = rhs._defaultClientBodySize;
    _errorPages = rhs._errorPages;
	_locationList = rhs._locationList;
    _defaultRoot = rhs._defaultRoot;
    _defaultAllow = rhs._defaultAllow;
    _defaultServerName = rhs._defaultServerName;
    _defaultIndex = rhs._defaultIndex;
    _defaultType = rhs._defaultType;
    _defaultCharset = rhs._defaultCharset;
    _defaultLanguage = rhs._defaultLanguage;
    _defaultAuth_basic = rhs._defaultAuth_basic;
    _defaultAuth_basic_user_file = rhs._defaultAuth_basic_user_file;
    _defaultAutoindex = rhs._defaultAutoindex;
	_defaultCgi = rhs._defaultCgi;
	_defaultCgi_allow = rhs._defaultCgi_allow;
	_defaultCgi_root = rhs._defaultCgi_root;
    return *this;
}

std::string             ConfigServer::getLocation(std::string uri)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;
    std::string location;
    size_t      length;

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        length = itBegin->first.length();
        if (itBegin->first[length - 1] == '*')
            location = itBegin->first.substr(0, length - 1);
        else
            location = itBegin->first;        
        if (uri.find(location) != std::string::npos)
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
        if (location.compare(itBegin->first) == 0 && (itBegin->second._root).compare(""))
            return (str.assign((itBegin->second._root)).append(itBegin->second._location));
        else if (location.compare(itBegin->first) == 0)
            break;
        itBegin++;
    }
    if (itBegin != itEnd)
        return (str.assign(_defaultRoot).append(itBegin->second._location));
    return _defaultRoot; 
}

std::vector<std::string>             ConfigServer::getServerName()
{
    return _defaultServerName;
}

std::vector<std::string>  &ConfigServer::getIndex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;    

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0 && itBegin->second._index.size() > 0)
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._type.compare(""))
            return (itBegin->second._type);
        itBegin++;
    }
    return _defaultType;
}

std::map<std::string, std::string>      &ConfigServer::getMimeTypes()
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._language.size() > 0)
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._allow.size() > 0)
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._charset.compare(""))
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._auth_basic.compare(""))
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._auth_basic_user_file.compare(""))
            return (itBegin->second._auth_basic_user_file);
        itBegin++;
    }
    return _defaultAuth_basic_user_file;   
}

int                 ConfigServer::getAutoindex(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0 && itBegin->second._autoindex != -1)
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._alias.compare(""))
            return (itBegin->second._alias);
        itBegin++;
    }
    return "";   
}

std::vector<std::string>             &ConfigServer::getCGI(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0 && itBegin->second._cgi.size() > 0)
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
        if (location.compare(itBegin->first) == 0 && itBegin->second._cgi_root.compare(""))
            return (itBegin->second._cgi_root);
        itBegin++;
    }
    return _defaultCgi_root;   
}

std::vector<std::string>	&ConfigServer::getCGI_allow(std::string location) {
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0 && itBegin->second._cgi_allow.size() > 0)
            return (itBegin->second._cgi_allow);
        itBegin++;
    }
    return _defaultCgi_allow;  
}

int                         ConfigServer::getClientBodySize(std::string location) {
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0 && itBegin->second._clientBodySize != -1)
            return (itBegin->second._clientBodySize);
        itBegin++;
    }
    return _defaultClientBodySize;  

}

std::vector<int>                         ConfigServer::getPort() {
    return _port;
}

std::map<int, std::string>  &ConfigServer::getErrorPages() {
    return _errorPages;
}

std::string                 ConfigServer::getHTMLErrorPage(int error) {
    std::map<int, std::string>::iterator it;
	std::string page;

    it = _errorPages.find(error);
	if (it == _errorPages.end())
		return ("");
	
	page.append(_errorFilesRoot);
	page.append(it->second);
    return page;
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

std::string             ConfigServer::getPutRoot()
{
    return _putRoot;
}

std::map<std::string, Location, Compare<std::string> > ConfigServer::getLocationList() {
    return _locationList;
}

void					ConfigServer::setCGI(std::vector<std::string> cgi) {
    _defaultCgi = cgi;
}

void					ConfigServer::setCGI_allow(std::vector<std::string> cgi_allow) {
    _defaultCgi_allow = cgi_allow;
}

void					ConfigServer::setCGI_root(std::string cgi_root) {
    _defaultCgi_root = cgi_root;
}

void                    ConfigServer::setPort(int port) {
    _port.push_back(port);
}

void                    ConfigServer::setServer_name(std::vector<std::string> server_name) {
    _defaultServerName = server_name;
}

void                    ConfigServer::setRoot(std::string root) {
    _defaultRoot = root;
}

void                    ConfigServer::setIndex(std::vector<std::string> index) {
    _defaultIndex = index;
}

void					ConfigServer::setType(std::string type) {
	_defaultType = type;
}

void					ConfigServer::setCharset(std::string charset) {
	_defaultCharset = charset;
}

void					ConfigServer::setLanguage(std::vector<std::string> language) {
	_defaultLanguage = language;
}


void                    ConfigServer::setAutoIndex(int autoIndex) {
    _defaultAutoindex = autoIndex;
}

void                    ConfigServer::setClientBodySize(int clientBodySize) {
    _defaultClientBodySize = clientBodySize;
}

void                    ConfigServer::setAllow(std::vector<std::string> allow) {
    _defaultAllow = allow;
}


void					ConfigServer::setAuth_basic(std::string auth) {
	_defaultAuth_basic = auth;
}

void					ConfigServer::setAuth_basic_user_file(std::string user_file) {
	_defaultAuth_basic_user_file = user_file;
}

void					ConfigServer::setErrorRoot(std::string root) {
	_errorFilesRoot = root;
}

void                    ConfigServer::setErrorPages(int error, std::string page) {
    _errorPages.insert(std::pair<int, std::string>(error, page));
}

void                    ConfigServer::insertLocation(std::string s, Location location) {
    _locationList.insert(std::pair<std::string, Location>(s, location));
}
