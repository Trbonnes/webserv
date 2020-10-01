#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <list>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include "../utils/utils.hpp"

class   Config
{
    private:

    std::string                 _root;
    std::vector<std::string>    _defaultAllow;
    std::string                 _serverName;
    std::list<std::string>      _index;
    std::string                 _defaultType;
    std::list<std::string>      _mimeTypes;
    std::string                 _configFiles;
    std::vector<std::string>    _defaultLanguage;

    public:

    Config();
    Config(Config &copy);
    ~Config();

    Config                      &operator=(Config const &rhs);

    std::string                 getRoot();
    std::string                 getServerName();
    std::list<std::string>      &getIndex();
    std::string                 getDefaultType();
    std::list<std::string>      &getMimeTypes();
    std::vector<std::string>    &getDefaultLanguage();
    std::vector<std::string>    &getDefaultAllow();
};

#endif