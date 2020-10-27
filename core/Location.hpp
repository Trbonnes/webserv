#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <list>

// Les locations doivent être triées par ordre de longueur
//    Du plus grand au plus petit

class   Location
{
    public:
    
    std::string                 _location;
    std::string                 _root;
    std::vector<std::string>    _allow;
    std::vector<std::string>    _index;
    std::string                 _type;
    std::string                 _charset;
    std::vector<std::string>    _language;
    std::string                 _auth_basic;
    std::string                 _auth_basic_user_file;
    bool                        _autoindex;
    std::string                 _alias;
    int                         _maxBody;


	std::vector<std::string>	_cgi;
	std::vector<std::string>	_cgi_allow;
	std::vector<std::string>	_cgi_methods;
	std::string					_cgi_root;

    Location(std::string location, std::string root, std::vector<std::string> allow, 
    std::vector<std::string> index, std::string type, std::string charset, std::vector<std::string> language,
    std::string auth_basic, std::string auth_basic_user_file, bool autoindex, std::string alias,
    std::vector<std::string> cgi, std::vector<std::string> cgi_methods, std::string cgi_root, int maxBody);
    ~Location();

    Location();
};

#endif