#include "Location.hpp"

Location::Location() : 
_location(""),
_root(""),
_allow(0),
_index(0),
_type(""),
_charset(""),
_language(0),
_auth_basic(""),
_auth_basic_user_file(""),
_autoindex(false),
_alias(""),
_cgi(),
_cgi_root("") {}

Location::Location(std::string location, std::string root, std::vector<std::string> allow,
std::vector<std::string> index, std::string type, std::string charset, std::vector<std::string> language,
std::string auth_basic, std::string auth_basic_user_file, bool autoindex, std::string alias,
std::vector<std::string> cgi, std::vector<std::string> cgi_methods, std::string cgi_root) :
_location(location),
_root(root),
_allow(allow),
_index(index),
_type(type),
_charset(charset),
_language(language),
_auth_basic(auth_basic),
_auth_basic_user_file(auth_basic_user_file),
_autoindex(autoindex),
_alias(alias),
_cgi(cgi),
_cgi_methods(cgi_methods),
_cgi_root(cgi_root) {}

Location::~Location() {}