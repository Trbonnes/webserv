#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>

class   Config
{
    protected:

    std::string     _root;
    std::string     _serverName;

    public:

    Config();
    Config(Config &copy);
    ~Config();

    Config          &operator=(Config const &rhs);

    std::string     getRoot();
    std::string     getServerName();
};

#endif
