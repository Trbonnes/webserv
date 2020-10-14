#include "Location.hpp"

Location::Location() : 
    _allow(0),
    _type(""),
    _charset(""),
	_language(0),
    _auth_basic(""),
    _auth_basic_user_file(""),

	_location(""),
	_cgi(0),
	_cgi_allow(0),
	_cgi_root(""),
    _root(""),
	_index(0),
    _autoindex(false),
    _clientBodySize(0),
	_alias("") {	
}

Location::Location(const Location &c) {
	_allow = c._allow;
    _type = c._type;
    _charset = c._charset;
	_language = c._language;
    _auth_basic = c._auth_basic;
    _auth_basic_user_file = c._auth_basic_user_file;

	_location = c._location;
	_cgi = c._cgi;
	_cgi_allow = c._cgi_allow;
	_cgi_root = c._cgi_root;
    _root = c._root;
	_index = c._index;
    _autoindex = c._autoindex;
    _clientBodySize = c._clientBodySize;
	_alias = c._alias;
}

Location::~Location() {

}

Location &Location::operator=(const Location &c) {
	_allow = c._allow;
    _type = c._type;
    _charset = c._charset;
	_language = c._language;
    _auth_basic = c._auth_basic;
    _auth_basic_user_file = c._auth_basic_user_file;

	_location = c._location;
	_cgi = c._cgi;
	_cgi_allow = c._cgi_allow;
	_cgi_root = c._cgi_root;
    _root = c._root;
	_index = c._index;
    _autoindex = c._autoindex;
    _clientBodySize = c._clientBodySize;
	_alias = c._alias;
	return *this;
}
