#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include <iostream>
# include <list>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include "Compare.hpp"
# include "Location.hpp"
# include "../utils/utils.hpp"

class   ConfigServer
{
    private:

    // Server Software 
    std::string                 _httpVersion;
    std::string                 _serverSoftware;
    std::list<std::string>      _mimeTypes;
    std::string                 _configFilesRoot;
    std::string                 _errorFilesRoot;

    // Default Server
    std::string                 _defaultPort;
    std::string                 _defaultServerName;

    // Config Server
    std::map<std::string, Location, Compare<std::string> > 
                                _locationList;
    std::string                 _defaultRoot;
    std::vector<std::string>    _defaultAllow;
    std::vector<std::string>    _defaultIndex;
    std::string                 _defaultType;
    std::string                 _defaultCharset;
    std::vector<std::string>    _defaultLanguage;
    std::string                 _defaultAuth_basic;
    std::string                 _defaultAuth_basic_user_file;
    bool                        _defaultAutoindex;
    int                         _defaultMaxBody;

    // CGI
	std::vector<std::string>	_defaultCgi;
	std::vector<std::string>	_defaultCgi_methods;
	std::string					_defaultCgi_root;

    // PUT
    std::string                 _putRequestRoot;

    public:

    ConfigServer();
    ConfigServer(ConfigServer &copy);
    ~ConfigServer();

    ConfigServer                &operator=(ConfigServer const &rhs);

    std::string                 getRoot(std::string _uri);
    std::string                 getServerName(void);
    std::string                 getPort(void);
    std::string                 getHttpVersion(void);
    std::string                 getServerSoftware(void);
    std::vector<std::string>    &getIndex(std::string location);
    std::string                 getType(std::string location);
    std::string                 getCharset(std::string location);
    std::vector<std::string>    &getLanguage(std::string location);
    std::vector<std::string>    &getAllow(std::string location);
    std::list<std::string>      &getMimeTypes(void);
    std::string                 getErrorFilesRoot(void);
    std::string                 getLocation(std::string location);
    std::string                 getAuth_basic(std::string location);
    std::string                 getAuth_basic_user_file(std::string location);
    bool                        getAutoindex(std::string location);
    std::string                 getAlias(std::string location);
    std::vector<std::string>    &getCGI(std::string location);
    std::vector<std::string>    &getCGImethods(std::string location);
    std::string                 getCGI_root(std::string location);
    int                         getClientBodySize(std::string location);
    std::string                 getPutRequestRoot(void);
};

#endif
