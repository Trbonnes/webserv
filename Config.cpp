#include "Config.hpp"

Config::Config() :
_root("/home/pauline/webserver/www") {}

Config::Config(Config &copy)
{
    _root = copy._root;
}

Config::~Config() {}

Config          &Config::operator=(Config const &rhs)
{
    _root = rhs._root;
    return *this;
}

std::string    Config::getRoot()
{
    return _root;
}

