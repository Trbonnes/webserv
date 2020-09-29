#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>

class   Config
{
    protected:

    std::string     _root;

    public:

    Config();
    Config(Config &copy);
    ~Config();

    Config          &operator=(Config const &rhs);

    std::string     getRoot();
};

#endif
