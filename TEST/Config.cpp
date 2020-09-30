#include "Config.hpp"

Config::Config() :
_root("/sgoinfre/goinfre/Perso/pganglof/webserver/www"),
_serverName("localhost"),
_index(0) {
    _index.push_back("index.php");
    _index.push_back("index.html");
}

Config::Config(Config &copy)
{
    _root = copy._root;
}

Config::~Config() {}

Config                  &Config::operator=(Config const &rhs)
{
    _root = rhs._root;
    return *this;
}

std::string             Config::getRoot()
{
    return _root;
}

std::string             Config::getServerName()
{
    return _serverName;
}

std::list<std::string>  Config::getIndex()
{
    return _index;
}