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
;
public:
    std::vector<std::string>    _allow;
    std::string                 _type;
    std::string                 _charset;
    std::vector<std::string>    _language;
    std::string                 _auth_basic;
    std::string                 _auth_basic_user_file;

	std::string					_location;
	std::vector<std::string>	_cgi;
	std::vector<std::string>	_cgi_allow;
	std::string					_cgi_root;
    std::string                 _root;
    std::vector<std::string>    _index;
    bool                        _autoindex;
    int                         _clientBodySize;
	std::string					_alias;

	Location();
	Location(const Location &c);
	/*virtual*/ ~Location();
	Location &operator=(const Location &c);

    //void                    printLocation(); ->used for local tests
};

#endif
