/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:13:35 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/26 20:45:38 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer() :
_defaultRoot(""),
// _defaultIndex(0),
// _defaultType(""),
// _defaultCharset(""),
// _defaultLanguage(0),
// _configFilesRoot(""),
// _defaultAuth_basic(""),
// _defaultAuth_basic_user_file(""),
// _defaultServerName(0),
// _defaultCgi(0),
// _defaultCgi_allow(0),
// _defaultCgi_root(""),
// _defaultAutoindex(0),
// _defaultAllow(0),
_defaultClientBodySize(-1)
// _port(0) 
{
    // _defaultAllow.push_back("GET");
    // _defaultAllow.push_back("HEAD");
    // _defaultAllow.push_back("POST");
    // _defaultAllow.push_back("PUT");
    // _defaultLanguage.push_back("fr");
    // _defaultLanguage.push_back("en");
    // _defaultIndex.push_back("index.html");
    // _defaultIndex.push_back("index.html");
    // _mimeTypes.push_back("plain/text");
    // _defaultServerName.push_back("localhost");

/*

    -----mime.types test not for configfile parsing-----

    // open mime.types **
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

    // Locations **

        // first location **

    std::list<std::string> index;
    
    index.push_back("index.php");

    Location loc1("/data/", "/home/pauline/webserver/www",
    _defaultAllow, index,
    "text/html", "utf-8", _defaultLanguage, _defaultAuth_basic, _defaultAuth_basic_user_file, _defaultAutoindex);

    _locationList["/data/"] = loc1;

        // second location **

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

void	Location::printLocation() {
	std::cout << std::endl << "\033[0;33m///LOCATION///" << std::endl;

	std::cout << "\033[0;34m--LOCATION--" << std::endl;
	std::cout << _location << std::endl;
	std::cout << "--ALIAS--" << std::endl;
	std::cout << _alias << std::endl;
	std::cout << "--ROOT--" << std::endl;
	std::cout << _root << std::endl;
	std::cout << "--INDEX--" << std::endl;
	if (_index.size())
		for (std::vector<std::string>::iterator it = _index.begin(); it != _index.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--AUTO INDEX--" << std::endl;
	std::cout << _autoindex << std::endl;
	std::cout << "--CLIENT BODY--" << std::endl;
	std::cout << _clientBodySize << std::endl;
	std::cout << "--ALLOW--" << std::endl;
	if (_allow.size())
		for (std::vector<std::string>::iterator it = _allow.begin(); it != _allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI--" << std::endl;
	if (_cgi.size())
		for (std::vector<std::string>::iterator it = _cgi.begin(); it != _cgi.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ALLOW--" << std::endl;
	if (_cgi_allow.size())
		for (std::vector<std::string>::iterator it = _cgi_allow.begin(); it != _cgi_allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ROOT--" << std::endl;
	std::cout << _cgi_root << std::endl;

}

void	ConfigServer::printServer() {
	std::cout << std::endl << "\033[0;31m///SERVER///" << std::endl;

	std::cout << "\033[0;32m--PORT--" << std::endl;
	std::cout << _port << std::endl;
	std::cout << "--SERVER NAME--" << std::endl;
	if (_defaultServerName.size())
		for (std::vector<std::string>::iterator it = _defaultServerName.begin(); it != _defaultServerName.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--ROOT--" << std::endl;
	std::cout << _defaultRoot << std::endl;
	std::cout << "--INDEX--" << std::endl;
	if (_defaultIndex.size())
		for (std::vector<std::string>::iterator it = _defaultIndex.begin(); it != _defaultIndex.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--AUTO INDEX--" << std::endl;
	std::cout << _defaultAutoindex << std::endl;
	std::cout << "--CLIENT BODY--" << std::endl;
	std::cout << _defaultClientBodySize << std::endl;
	std::cout << "--ALLOW--" << std::endl;
	if (_defaultAllow.size())
		for (std::vector<std::string>::iterator it = _defaultAllow.begin(); it != _defaultAllow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI--" << std::endl;
	if (_defaultCgi.size())
		for (std::vector<std::string>::iterator it = _defaultCgi.begin(); it != _defaultCgi.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ALLOW--" << std::endl;
	if (_defaultCgi_allow.size())
		for (std::vector<std::string>::iterator it = _defaultCgi_allow.begin(); it != _defaultCgi_allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ROOT--" << std::endl;
	std::cout << _defaultCgi_root << std::endl;

	std::cout << "--ERROR PAGES--" << std::endl;
	for (auto& x: _errorPages)
		std::cout << " [" << x.first << ':' << x.second << ']';
	std::cout << '\n';
	std::cout << "404 Error code: " << getHTMLErrorPage(404) << std::endl;
	std::cout << "202 Error code: " << getHTMLErrorPage(202) << std::endl;

	std::cout << "\033[0;35mLocation List: " << _locationList.size() << std::endl;
	
	std::cout << "Locations :";
	for (auto& x: _locationList)
		std::cout << " [" << x.first << ':' << x.second._location << ']';
	std::cout << '\n';

	for (std::map<std::string, Location, Compare<std::string> >::iterator it = _locationList.begin(); it != _locationList.end(); it++) {
			it->second.printLocation();
	}
}

ConfigServer::ConfigServer(const ConfigServer &copy)
{
    _locationList = copy._locationList;
    _defaultRoot = copy._defaultRoot;
    _defaultAllow = copy._defaultAllow;
    _defaultServerName = copy._defaultServerName;
    _defaultIndex = copy._defaultIndex;
    _defaultType = copy._defaultType;
    _defaultCharset = copy._defaultCharset;
    _defaultLanguage = copy._defaultLanguage;
    _mimeTypes = copy._mimeTypes;
    _configFilesRoot = copy._configFilesRoot;
    _defaultAuth_basic = copy._defaultAuth_basic;
    _defaultAuth_basic_user_file = copy._defaultAuth_basic_user_file;
    _defaultAutoindex = copy._defaultAutoindex;
	_defaultCgi = copy._defaultCgi;
	_defaultCgi_allow = copy._defaultCgi_allow;
	_defaultCgi_root = copy._defaultCgi_root;
    _port = copy._port;
    _defaultClientBodySize = copy._defaultClientBodySize;
    _errorPages = copy._errorPages;
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
    _defaultCharset = rhs._defaultCharset;
    _defaultLanguage = rhs._defaultLanguage;
    _mimeTypes = rhs._mimeTypes;
    _configFilesRoot = rhs._configFilesRoot;
    _defaultAuth_basic = rhs._defaultAuth_basic;
    _defaultAuth_basic_user_file = rhs._defaultAuth_basic_user_file;
    _defaultAutoindex = rhs._defaultAutoindex;
	_defaultCgi = rhs._defaultCgi;
	_defaultCgi_allow = rhs._defaultCgi_allow;
	_defaultCgi_root = rhs._defaultCgi_root;
    _port = rhs._port;
    _defaultClientBodySize = rhs._defaultClientBodySize;
    _errorPages = rhs._errorPages;

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

bool                 ConfigServer::getAutoindex(std::string location)
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

std::vector<std::string>             ConfigServer::getCGI(std::string location)
{
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

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

std::vector<std::string>	&ConfigServer::getCGI_allow(std::string location) {
    std::map<std::string, Location, Compare<std::string> >::iterator itBegin;
    std::map<std::string, Location, Compare<std::string> >::iterator itEnd;  

    itBegin = _locationList.begin();
    itEnd = _locationList.end();
    while (itBegin != itEnd)
    {
        if (location.compare(itBegin->first) == 0)
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
        if (location.compare(itBegin->first) == 0)
            return (itBegin->second._clientBodySize);
        itBegin++;
    }
    return _defaultClientBodySize;  

}

int                         ConfigServer::getPort() {
    return _port;
}

std::map<int, std::string>  ConfigServer::getErrorPages() {
    return _errorPages;
}

std::string                 ConfigServer::getHTMLErrorPage(int error) {
    std::map<int, std::string>::iterator it;

    it = _errorPages.find(error);
	if (it == _errorPages.end())
		return ("");
    return it->second;
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
    _port = port;
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

void                    ConfigServer::setAutoIndex(bool autoIndex) {
    _defaultAutoindex = autoIndex;
}

void                    ConfigServer::setClientBodySize(int clientBodySize) {
    _defaultClientBodySize = clientBodySize;
}

void                    ConfigServer::setAllow(std::vector<std::string> allow) {
    _defaultAllow = allow;
}

void                    ConfigServer::setErrorPages(int error, std::string page) {
    _errorPages.emplace(error, page);
}

void                    ConfigServer::insertLocation(std::string s, Location location) {
    _locationList.emplace(s, location);
}
