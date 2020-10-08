#ifndef CONFIG_HPP
# define CONFIG_HPP

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

class   Config
{
    private:

    std::map<std::string, Location, CompLength<std::string> > _locationList;
    std::string                 _defaultRoot;
    std::vector<std::string>    _defaultAllow;
    std::string                 _serverName;
    int                         _port;
    std::list<std::string>      _defaultIndex;
    std::string                 _defaultType;
    std::string                 _defaultCharset;
    std::vector<std::string>    _defaultLanguage;
    std::list<std::string>      _mimeTypes;
    std::string                 _configFilesRoot;
    std::string                 _defaultAuth_basic;
    std::string                 _defaultAuth_basic_user_file;
    std::string                 _defaultAutoindex;

    public:

    Config();
    Config(std::map<std::string, Location, CompLength<std::string> > locationList, std::string defaultRoot, std::vector<std::string> defaultAllow, std::string serverName,
    int port, std::list<std::string> defaultIndex, std::string defaultType, std::string defaultCharset, std::vector<std::string> defaultLanguage,
    std::list<std::string> mimeTypes, std::string configFilesRoot, std::string defaultAuth_basic, std::string defaultAuth_basic_user_file,
    std::string defaultAutoindex);
    Config(Config const &copy);
    ~Config();

    Config                      &operator=(Config const &rhs);

    std::string                 getRoot(std::string _uri);
    std::string                 getServerName(std::string location);
    std::list<std::string>      &getIndex(std::string location);
    std::string                 getType(std::string location);
    std::string                 getCharset(std::string location);
    std::vector<std::string>    &getLanguage(std::string location);
    std::vector<std::string>    &getAllow(std::string location);
    std::list<std::string>      &getMimeTypes();
    std::string                 getLocation(std::string location);
    std::string                 getAuth_basic(std::string location);
    std::string                 getAuth_basic_user_file(std::string location);
    std::string                 getAutoindex(std::string location);
};

#endif
