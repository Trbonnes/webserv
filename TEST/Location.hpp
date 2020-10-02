#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <list>

class   Location
{
    public:
    
    std::string                 _location;
    std::string                 _root;
    std::vector<std::string>    _allow;
    std::string                 _serverName;
    std::list<std::string>      _index;
    std::string                 _type;
    std::vector<std::string>    _language;

    Location(std::string location, std::string root, std::vector<std::string> allow,
    std::string serverName, std::list<std::string> index, std::string type, std::vector<std::string> language);
    ~Location();

    private:

    Location();
};

#endif
