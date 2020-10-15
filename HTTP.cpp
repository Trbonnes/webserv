#include "HTTP.hpp"

const std::string HTTP::_base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

HTTP::HTTP() :
_socket(0),
_config(),
_cgi(),
_stat(),
_response(0),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_contentLength(0),
_charset(""),
_contentLocation(""),
_contentType(""),
_date(""),
_retryAfter(""),
_transferEncoding(""),
_body("") {
    _methodsName[GET] = "GET"; 
    _methodsName[HEAD] = "HEAD"; 
}

HTTP::HTTP(Socket &socket, ConfigServer &config) :
_socket(socket),
_config(config),
_cgi(),
_stat(),
_response(0),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_charset(""),
_contentLength(0),
_contentLocation(""),
_contentType(""),
_date(""),
_retryAfter(""),
_transferEncoding(""),
_body("")
{
    _methodsName[GET] = "GET"; 
    _methodsName[HEAD] = "HEAD"; 

    int         i;
    size_t      extension;
    std::string str;
    int         fd; 

    _uri = socket.getRequestURI();
    setLocation();
    replaceURI(); 
    i = 0;
    while (i < NB_METHODS)
    {
        if (_socket.getMethod().compare(_methodsName[i]) == 0)
        {
            if (checkAllowMethods(_methodsName[i]))
            {
                if (_config.getCGI_root(_location).length() > 0)
                {
                    fd = setRoot(); // setRoot a modifier, juste setRoot et pas open
                    close(fd);
                    cgi_metaVariables();
                    extension = _route.find_last_of('.');
                    if (is_good_exe(str.assign(_route).erase(0, extension + 1)))
                        cgi_exe();
                    else
                        _statusCode = BAD_REQUEST;
                }
                else
                    callMethod(i);
                return ;
            }
            else
                return ;
        }
        i++;
    }
    _statusCode = BAD_REQUEST;
}

HTTP::HTTP(HTTP &copy)
{
    _socket = copy._socket;
    _config = copy._config;
    _response = copy._response;
    _stat = copy._stat;
    _allow = copy._allow;
    _wwwAuthenticate = copy._wwwAuthenticate;
    _referer = copy._referer;
    ft_strcpy(copy._lastModified, _lastModified);
    _location = copy._location;
    _server = copy._server;
    _contentLanguage = copy._contentLanguage;
    _contentLength = copy._contentLength;
    _contentLocation = copy._contentLocation;
    _contentType = copy._contentType;
    ft_strcpy(copy._date, _date);
    _retryAfter = copy._retryAfter;
    _transferEncoding = copy._transferEncoding;
    _body = copy._body;
    for (int i = 0; i < NB_METHODS; i++)
    {
        _methodsName[i].assign(copy._methodsName[i]);
        _method[i] = copy._method[i];
    }
}

HTTP::~HTTP()
{
    int     i;

    i = 0;
    while (i < NB_METAVARIABLES)
        free(_cgi_env[i++]);
}

HTTP     &HTTP::operator=(HTTP &rhs)
{
    _socket = rhs._socket;
    _config = rhs._config;
    _response = rhs._response;
    _stat = rhs._stat;
    _allow = rhs._allow;
    _wwwAuthenticate = rhs._wwwAuthenticate;
    _referer = rhs._referer;
    ft_strcpy(rhs._lastModified, _lastModified);
    _location = rhs._location;
    _server = rhs._server;
    _contentLanguage = rhs._contentLanguage;
    _contentLength = rhs._contentLength;
    _contentLocation = rhs._contentLocation;
    _contentType = rhs._contentType;
    ft_strcpy(rhs._date, _date);
    _retryAfter = rhs._retryAfter;
    _transferEncoding = rhs._transferEncoding;
    _body = rhs._body;
    for (int i = 0; i < NB_METHODS; i++)
    {
        _methodsName[i].assign(rhs._methodsName[i]);
        _method[i] = rhs._method[i];
    }
    return *this;
}

void        HTTP::callMethod(int method)
{
    if (method == GET)
        get();
    else if (method == HEAD)
        head();
}

int         HTTP::checkAllowMethods(std::string method)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;
    int ret;

    ret = 0;
    itBegin = _config.getAllow(_location).begin();
    itEnd = _config.getAllow(_location).end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            ret = 1;
        itBegin++;
    }
    if (!ret)
      _statusCode = METHOD_NOT_ALLOWED;
    return (ret);
}

//** replace URI by the location **
void        HTTP::replaceURI()
{
  _uri.assign(_config.getRoot(_location));
}

int         HTTP::openFile()
{
    int         fd;
    struct stat file;
    std::string str;
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;

    fd = -1;
    itIndexBegin = _config.getIndex(_location).begin();
    itIndexEnd = _config.getIndex(_location).end();
    stat(_route.c_str(), &file);
    str.assign(_route);
    while (itIndexBegin != itIndexEnd && (file.st_mode & S_IFMT) != S_IFREG)
    {
        str.assign(_route);
        if (str.back() != '/')
            str.append("/");
        str.append(*itIndexBegin);
        stat(str.c_str(), &file);
        itIndexBegin++;
    }
    _route.assign(str);
    if ((file.st_mode & S_IFMT) == S_IFREG)
    {
        fd = open(_route.c_str(), O_RDONLY);
       _statusCode = OK;
    }
    else
        _statusCode = FORBIDDEN;
    return fd;
}

void            HTTP::setAutoindex(void)
{
    std::string     str;
    struct stat     directory;
    DIR             *dir;
    struct dirent   *dirent;
    struct tm       *timeinfo;
    char            lastModifications[100];
    std::stack<std::string> files;

    _body.assign("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
    dir = opendir(_uri.c_str());
    if (dir == NULL)
        _statusCode = INTERNAL_SERVER_ERROR;
    else
    {
        while ((dirent = readdir(dir)) != NULL)
        {
            stat(str.assign(_uri).append(dirent->d_name).c_str(), &directory);
            if (str.assign(dirent->d_name).compare(".") == 0)
                continue ;
            str.assign("<a href=\"");
            str.append(dirent->d_name);
            if (dirent->d_type == DT_DIR)
                str.append("/");
            str.append("\">");
            str.append(dirent->d_name);
            if (dirent->d_type == DT_DIR)
                str.append("/");
            str.append("</a>\t\t\t\t");
            timeinfo = localtime(&(directory.st_mtime)); // st_mtimespec.tv.sec = macos ; st_mtime = linux
            strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
            str.append(lastModifications);
            str.append("\t\t");
            if (dirent->d_type == DT_DIR)
                str.append("-");
            else
                str.append(std::to_string(directory.st_size));
            str.append("\n");
            files.push(str);
        }
        while (!files.empty())
        {
            _body.append(files.top());
            files.pop();
        }
    }
    _body.append("</pre><hr></body>\n</html>\n");
}

void            HTTP::authorization()
{
    int     fd;
    int     ret;
    char    *line;

    if (_config.getAuth_basic(_location).compare("off") != 0)
    {
        if (_socket.getAuthorization().compare("") == 0)
        {
            _statusCode = UNAUTHORIZED;
            _wwwAuthenticate.assign("Basic realm=").append(_config.getAuth_basic(_location));
        }
        else
        {
            if ((fd = open(_config.getAuth_basic_user_file(_location).c_str(), O_RDONLY)) >= 0)
            {
                while ((ret = get_next_line(fd, &line)) > 0)
                {
                    if (base64_decode(_socket.getAuthorization()).compare(line) == 0)
                    {
                        _wwwAuthenticate.assign("OK");
                        break ;
                    }
                }
                if (base64_decode(_socket.getAuthorization()).compare(line) == 0)
                    _wwwAuthenticate.assign("OK");
                if (_wwwAuthenticate.compare("OK") != 0)
                {
                    _statusCode = UNAUTHORIZED;
                    _wwwAuthenticate.assign("Basic realm=").append(_config.getAuth_basic(_location));
                }
                free(line);
            }
        }
    }
    else
        _wwwAuthenticate.assign("OK");
}

inline bool   HTTP::is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

//** Encode password in base64 **
std::string   HTTP::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
  std::string   ret;
  int           i;
  int           j;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  i = 0;
  j = 0;
  while (in_len--)
  {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for(i = 0; (i <4) ; i++)
        ret += _base64_chars[char_array_4[i]];
      i = 0;
    }
  }
  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    for (j = 0; (j < i + 1); j++)
      ret += _base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }
  return ret;
}

//** Decode password in base64 **
std::string   HTTP::base64_decode(std::string const& encoded_string) {
  int           in_len;
  int           i;
  int           j;
  int           in_;
  unsigned char char_array_4[4];
  unsigned char char_array_3[3];
  std::string   ret;

  in_len = encoded_string.size();
  i = 0;
  j = 0;
  in_ = 0;
  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
  {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = _base64_chars.find(char_array_4[i]);
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }
  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;
    for (j = 0; j <4; j++)
      char_array_4[j] = _base64_chars.find(char_array_4[j]);
    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
    for (j = 0; (j < i - 1); j++)
      ret += char_array_3[j];
  }
  return ret;
}

int         HTTP::getResponse()
{
    std::vector<std::string>::iterator it;
    std::cout << std::endl;

    std::cout << "ORIGINALE URI: " << _socket.getRequestURI() << std::endl << std::endl;
    std::cout << "LOCATION: " << _location << std::endl << std::endl;
    std::cout << "URI: " << _uri << std::endl << std::endl;
    std::cout << "ROUTE: " << _route << std::endl << std::endl;
    std::cout << "CONTENT-LOCATION: " << _contentLocation << std::endl << std::endl;
    std::cout << "SERVER NAME: " << _server << std::endl << std::endl;
    std::cout << "STATUS CODE: " << _statusCode << std::endl << std::endl;

    it = _allow.begin();
    std::cout << "ALLOW: ";
    while (it != _allow.end())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl << std::endl;
    std::cout << "CONTENT TYPE: " << _contentType << std::endl << std::endl;
    std::cout << "CHARSET: " << _charset << std::endl << std::endl;
    std::cout << "CONTENT LENGTH: " << _contentLength << std::endl << std::endl;
    std::cout << "CONTENT LANGUAGE: " << _contentLanguage << std::endl << std::endl;
    std::cout << "WWW AUTHENTICATE: " << _wwwAuthenticate << std::endl << std::endl;
    std::cout << "LAST MODIFIED: " << _lastModified << std::endl << std::endl;
    std::cout << "DATE: " << _date << std::endl << std::endl;
    std::cout << "BODY: " << std::endl << _body << std::endl << std::endl;

    return (1);
}
