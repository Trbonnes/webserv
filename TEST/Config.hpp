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

    std::string                 _defaultRoot;
    std::vector<std::string>    _defaultAllow;
    std::string                 _defaultServerName;
    std::list<std::string>      _defaultIndex;
    std::string                 _defaultType;
    std::vector<std::string>    _defaultLanguage;
    std::list<std::string>      _mimeTypes;
    std::string                 _configFilesRoot;

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
