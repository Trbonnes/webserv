#include "Location.hpp"

Location::Location() : 
_location(""),
_root(""),
_allow(0),
_serverName(""),
_index(0),
_type(""),
_language(0) {}

Location::Location(std::string location, std::string root, std::vector<std::string> allow,
std::string serverName, std::list<std::string> index, std::string type, std::vector<std::string> language) :
_location(location),
_root(root),
_allow(allow),
_serverName(serverName),
_index(index),
_type(type),
_language(language) {}

Location::~Location() {}