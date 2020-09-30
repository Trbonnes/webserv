#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <list>

class   Config
{
    private:

    std::string             _root;
    std::string             _serverName;
    std::list<std::string>  _index;

    public:

    Config();
    Config(Config &copy);
    ~Config();

    Config                  &operator=(Config const &rhs);

    std::string             getRoot();
    std::string             getServerName();
    std::list<std::string>  getIndex();
};

#endif
