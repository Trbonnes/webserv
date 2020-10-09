#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <list>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include <map>
class	Location {
private:
    std::vector<std::string>    _allow;
    std::string                 _type;
    std::string                 _charset;
    std::vector<std::string>    _language;
    std::string                 _auth_basic;
    std::string                 _auth_basic_user_file;

	//PARSER VARIABLES
	std::string					_location;
	std::vector<std::string>	_cgi;
	std::vector<std::string>	_cgi_method;
	std::string					_cgi_root;
    std::string                 _root;
    std::vector<std::string>    _index;
    bool                        _autoIndex;
    int                         _clientBodySize;
    std::vector<std::string>    _acceptedMethods;
	std::string					_alias;
	//
public:
	Location();
	Location(const Location &c);
	/*virtual*/ ~Location();
	Location &operator=(const Location &c);


    std::vector<std::string>    &getAllow();
    std::string                 getType();
    std::string                 getCharset();
    std::vector<std::string>    &getLanguage();
    std::string                 getAuth_basic();
    std::string                 getAuth_basic_user_file();

	//PARSER VARIABLES GETTERS
	std::string					getLocation();
	std::vector<std::string>	getCgi();
	std::vector<std::string>	getCgi_method();
	std::string					getCgi_root();
    std::string                 getRoot();
    std::vector<std::string>    &getIndex();
    bool                        getAutoIndex();
    int                         getClientBodySize();
    std::vector<std::string>    getAcceptedMethods();
	std::string					getAlias();
	//

	//PARSER VARIABLES SETTERS
	void						setLocation(std::string _location);
	void						setCgi(std::vector<std::string> _cgi);
	void						setCgi_method(std::vector<std::string> _cgi_method);
	void						setCgi_root(std::string _cgi_root);
    void						setRoot(std::string _root);
    void						setIndex(std::vector<std::string> _index);
    void						setAutoIndex(bool _autoIndex);
    void						setClientBodySize(int _clientBodySize);
    void						setAcceptedMethods(std::vector<std::string> _acceptedMethods);
	void						setAlias(std::string _alias);
	//
};

#endif
