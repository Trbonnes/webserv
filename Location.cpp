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
	_cgi_method(0),
	_cgi_root(""),
    _root(""),
	_index(0),
    _autoIndex(false),
    _clientBodySize(0),
	_acceptedMethods(0),
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
	_cgi_method = c._cgi_method;
	_cgi_root = c._cgi_root;
    _root = c._root;
	_index = c._index;
    _autoIndex = c._autoIndex;
    _clientBodySize = c._clientBodySize;
	_acceptedMethods = c._acceptedMethods;
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
	_cgi_method = c._cgi_method;
	_cgi_root = c._cgi_root;
    _root = c._root;
	_index = c._index;
    _autoIndex = c._autoIndex;
    _clientBodySize = c._clientBodySize;
	_acceptedMethods = c._acceptedMethods;
	_alias = c._alias;
	return *this;
}

std::vector<std::string>    &Location::getAllow() {
	return _allow;	
}

std::string                 Location::getType() {
	return _type;	
}

std::string                 Location::getCharset() {
	return _charset;	
}

std::vector<std::string>    &Location::getLanguage() {
	return _language;	
}

std::string                 Location::getAuth_basic() {
	return _auth_basic;	
}

std::string                 Location::getAuth_basic_user_file() {
	return _auth_basic_user_file;	
}


std::string					Location::getLocation() {
	return _location;	
}

std::vector<std::string>	Location::getCgi() {
	return _cgi;	
}

std::vector<std::string>	Location::getCgi_method() {
	return _cgi_method;	
}

std::string					Location::getCgi_root() {
	return _cgi_root;	
}

std::string                 Location::getRoot() {
	return _root;	
}

std::vector<std::string>    &Location::getIndex() {
	return _index;	
}

bool                        Location::getAutoIndex() {
	return _autoIndex;	
}

int                         Location::getClientBodySize() {
	return _clientBodySize;	
}

std::vector<std::string>    Location::getAcceptedMethods() {
	return _acceptedMethods;	
}

std::string					Location::getAlias() {
	return _alias;	
}


void						Location::setLocation(std::string location) {
	_location = location;
}

void						Location::setCgi(std::vector<std::string> cgi) {
	_cgi = cgi;
}

void						Location::setCgi_method(std::vector<std::string> cgi_method) {
	_cgi_method = cgi_method;
}

void						Location::setCgi_root(std::string cgi_root) {
	_cgi_root = cgi_root;
}

void						Location::setRoot(std::string root) {
	_root = root;
}

void						Location::setIndex(std::vector<std::string> index) {
	_index = index;
}

void						Location::setAutoIndex(bool autoIndex) {
	_autoIndex = autoIndex;
}

void						Location::setClientBodySize(int clientBodySize) {
	_clientBodySize = clientBodySize;
}

void						Location::setAcceptedMethods(std::vector<std::string> acceptedMethods) {
	_acceptedMethods = acceptedMethods;
}

void						Location::setAlias(std::string alias) {
	_alias = alias;
}
