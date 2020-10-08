#include "Socket.hpp"
#include "../Methods.hpp"
#include "Config.hpp"

int     main(void)
{
    Socket  socket;

    std::vector<std::string> _defaultAllow;
    _defaultAllow.push_back("GET");
    _defaultAllow.push_back("HEAD");
    _defaultAllow.push_back("POST");
    _defaultAllow.push_back("PUT");

    std::list<std::string> _defaultIndex;
    _defaultIndex.push_back("index.html");
    _defaultIndex.push_back("index.php");
    
    std::vector<std::string> _defaultLanguage;
    _defaultLanguage.push_back("fr");
    _defaultLanguage.push_back("en");

    std::list<std::string> _mimeTypes;
    int         ret;
    int         fd;
    char        *line;
    std::string string;
    std::string file;
    std::list<std::string>::iterator it;
    std::string::iterator s_it;

    file.append("/home/pauline/webserver/config");
    file.append("/mime.types");
    if ((fd = open(file.c_str(), O_RDONLY)) >= 0)
    {
        while ((ret = get_next_line(fd, &line)) > 0)
        {
            string = line;
            _mimeTypes.push_back(string);
        }
        string = line;
        _mimeTypes.push_back(string);
        it = _mimeTypes.begin();
        while (it != _mimeTypes.end())
        {
            s_it = (*it).begin();
            while (*s_it == ' ')
                s_it++;
            (*it).erase((*it).begin(), s_it);
            it++;
        }
        free(line);
        close (fd);
    }

    // Revoir root des locations

    std::map<std::string, Location, CompLength<std::string> > _locationList1;

    std::list<std::string> index;
    
    index.push_back("index.php");

    Location loc1("/data/", "/home/pauline/webserver/www",
    _defaultAllow, index,
    "text/html", "utf-8", _defaultLanguage, "\"Authorization\"", "/home/pauline/webserver/config/.htpasswd", "off");

    _locationList1["/data/"] = loc1;

    std::list<std::string> index2;

    index2.push_back("42.png");

    Location loc2("/images/", "/home/pauline/webserver/www",
    _defaultAllow, index2,
    "text/html", "", _defaultLanguage, "off", "", "off");

    _locationList1["/images/"] = loc2;

    Location loc3("/", "/home/pauline/webserver/www",
    _defaultAllow, _defaultIndex,
    "text/plain", "koi8-r", _defaultLanguage, "off", "", "on");

    _locationList1["/"] = loc3;


    std::map<std::string, Location, CompLength<std::string> > _locationList2;

    std::list<std::string> index3;
    
    index.push_back("index.php");

    Location loc4("/data/", "/home/pauline/webserver/www2",
    _defaultAllow, index3,
    "text/html", "utf-8", _defaultLanguage, "\"Authorization\"", "/home/pauline/webserver/config/.htpasswd", "off");

    _locationList2["/data/"] = loc4;

    std::list<std::string> index4;

    index2.push_back("42.png");

    Location loc5("/images/", "/home/pauline/webserver/www2",
    _defaultAllow, index4,
    "text/html", "", _defaultLanguage, "off", "", "off");

    _locationList2["/images/"] = loc5;

    Location loc6("/", "/home/pauline/webserver/www2",
    _defaultAllow, _defaultIndex,
    "text/plain", "koi8-r", _defaultLanguage, "off", "", "on");

    _locationList2["/"] = loc6;


    std::map<std::string, Config, CompNormalOrder<std::string> > servers;

    Config server1(_locationList1, "/www", _defaultAllow, "localhost", 80, _defaultIndex, "text/plain", "koi8-r", _defaultLanguage, _mimeTypes, "/home/pauline/webserver/config", "\"Authorization\"", "/home/pauline/webserver/config/.htpasswd", "off");
    Config server2(_locationList2, "/www2", _defaultAllow, "127.0.0.2", 5000, _defaultIndex, "text/plain", "koi8-r", _defaultLanguage, _mimeTypes, "/home/pauline/webserver/config", "\"Authorization\"", "/home/pauline/webserver/config/.htpasswd", "off");

    servers["localhost"] = server1;
    servers["127.0.0.2"] = server2;


    Methods method(servers, socket);

    method.getResponse();

    return 0;
}
