#ifndef CONFIG_HPP
# define CONFIG_HPP

class   Config
{
    private:

    std::string     _root;

    public:
    Config();
    Config(Config &config);
    ~Config();

    Config      &operator=(Config const &rhs);

    std::string     getRoot();
};

#endif
