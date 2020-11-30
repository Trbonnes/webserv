#include "HTTP.hpp"

const std::string HTTP::_base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

HTTP::HTTP() :
_socket(0),
_config(),
_mapCodes(),
_cgi(),
_location(""),
_stat(),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_date(""),
_server(""),
_contentLanguage(""),
_contentLength(0),
_contentLocation(""),
_contentType(""),
_charset(""),
_retryAfter(""),
_transferEncoding(""),
_body(),
_response() {}

HTTP::HTTP(Socket *socket, ConfigServer &config) :
_socket(*socket),
_config(config),
_mapCodes(),
_cgi(),
_location(""),
_stat(),
_statusCode(OK),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_date(""),
_server(""),
_contentLanguage(""),
_contentLength(0),
_contentLocation(""),
_contentType(""),
_charset(""),
_retryAfter(""),
_transferEncoding(""),
_body(),
_response()
{
    size_t      extension;
    std::string str;

    ft_bzero(_cgi_env, sizeof(_cgi_env));
    if (checkRequestErrors() != OK)
        return ;
    _uri = _socket.getRequestURI();
    setLocation();
    if (_config.getClientBodySize(_location) != -1 && (int)_socket.getBody().length() > _config.getClientBodySize(_location)) // TO DO quick fix
    {
        _statusCode = REQUEST_ENTITY_TOO_LARGE;
        return ;
    }
    setRoot();
    setStat();
    if (_socket.getMethod().compare("OPTIONS") == 0)
    {
        _statusCode = NO_CONTENT;
        return ;
    }
    extension = _route.find_last_of('.');
    if (is_good_exe(str.assign(_route).erase(0, extension + 1)) && checkCGImethods(_socket.getMethod()))
    {
        cgi_metaVariables();
        cgi_exe();
        setDate();
    }
    else if (checkAllowMethods(_socket.getMethod()))
        callMethod(_socket.getMethod());
    else
        _statusCode = METHOD_NOT_ALLOWED;
}

HTTP::HTTP(HTTP &copy)
{
    _socket = copy._socket;
    _config = copy._config;
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
    _body = ft_strdup(copy._body);
}

HTTP::~HTTP()
{
    int     i;

    i = 0;
    while (i < NB_METAVARIABLES)
        free(_cgi_env[i++]);
    free(_body);
    free(_response);
}

HTTP     &HTTP::operator=(HTTP &rhs)
{
    _socket = rhs._socket;
    _config = rhs._config;
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
    _body = ft_strdup(rhs._body);
    return *this;
}

//** Call the non CGI methods, GET, HEAD and PUT / OPTIONS is managed in a different way **  //
void        HTTP::callMethod(std::string method)
{
    if (method.compare("GET") == 0 || method.compare("HEAD") == 0)
        get();
    else if (method.compare("PUT") == 0)
        put();
    else if (method.compare("DELETE") == 0)
        del();
}

//** Check request errors **
int         HTTP::checkRequestErrors()
{
    if (_socket.getBody().length() > 0 && _socket.getContentLength().length() == 0 && _socket.getTransferEncoding().length() == 0)
        _statusCode = LENGTH_REQUIRED;
    return (_statusCode);
}

//** Check if the method is authorized for the non CGI locations **
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
        if ((*itBegin).compare(method) == 0 || method.compare("HEAD") == 0)
            ret = 1;
        itBegin++;
    }
    if (!ret)
      _statusCode = METHOD_NOT_ALLOWED;
    return (ret);
}

//** Absolute location route for the server **
void        HTTP::setLocation()
{
  _location = _config.getLocation(_uri);
}

//** Replace URI by the location **
void        HTTP::replaceURI()
{
    _uri.assign(_config.getRoot(_location));
}

//** Set stat to know all the details of the requested file **
void        HTTP::setStat()
{
    stat(_route.c_str(), &_stat);
}

//** Open file **
int         HTTP::openFile()
{
    int         fd;
    struct stat file;

    fd = -1;
    stat(_route.c_str(), &file);
    //if ((file.st_mode & S_IFMT) == S_IFREG)
    if (S_ISREG(file.st_mode))
    {
        fd = open(_route.c_str(), O_RDONLY);
        _statusCode = OK;
    }
    else
        _statusCode = FORBIDDEN;
    return fd;
}

// ** Set the root with language and location directory if needed **
void         HTTP::setRoot()
{
    int         fd;
    int         find;
    std::string str;
    struct stat file;
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;

    if (_uri.compare(0, 4, "http") == 0)
    {
        //** Absolute path **
        find = _route.append(_socket.getRequestURI()).find(_config.getServerName()[0]); // TO DO temp fix for compilation
        _route.erase(0, find + _config.getServerName()[0].length()); // TO DO quick fix
        _route.insert(0, _config.getRoot(_location));
        _route.insert(_config.getRoot(_location).length(), acceptLanguage());
    }
    else
    {
        replaceURI(); 

        //** Relative path **
        _route.assign(_config.getRoot(_location));
        _route.append(_socket.getRequestURI());
        stat(_route.c_str(), &file);

        // ** If file exist or put request, return **
        if (((S_ISREG(file.st_mode) && (fd = open(_route.c_str(), O_RDONLY)) != -1)) || _socket.getMethod().compare("PUT") == 0
            || (((file.st_mode & S_IFMT) == S_IFDIR) && _socket.getMethod().compare("DELETE") == 0))
        {
            close(fd);
            return ;
        }

        // ** Else, add the language **
        _route.assign(_config.getRoot(_location));
        _route.append(acceptLanguage());
        _route.append(str.assign(_socket.getRequestURI()).erase(0, _location.length()));
        stat(_route.c_str(), &file);
        
        // ** If file exist or delete request, return **
        if ((((file.st_mode & S_IFMT) == S_IFREG  && (fd = open(_route.c_str(), O_RDONLY)) != -1)) || _socket.getMethod().compare("DELETE") == 0)
        {
            close(fd);
            return ;
        }

        // ** Else, add index if it is not a put or delete request **
        itIndexBegin = _config.getIndex(_location).begin();
        itIndexEnd = _config.getIndex(_location).end();
        stat(_route.c_str(), &file);
        str.assign(_route);
        while (itIndexBegin != itIndexEnd && (file.st_mode & S_IFMT) != S_IFREG)
        {
            str.assign(_route);
            if (str.at(str.length() - 1) != '/')
                str.append("/");
            str.append(*itIndexBegin);
            stat(str.c_str(), &file);
            itIndexBegin++;
        }
        _route.assign(str);
    }
    return ;
}

// ** Create the default html page when file is not found and autoindex is on **
void            HTTP::setAutoindex(void)
{
    std::string             str;
    struct stat             directory;
    DIR                     *dir;
    struct dirent           *dirent;
    struct tm               *timeinfo;
    char                    lastModifications[100];
    std::stack<std::string> files;
    std::string             body;

    // Verifier que DIR marche bien sur linux, car bug sur MACOS

    body.assign("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
    dir = opendir(_route.c_str());
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
			#if defined(TARGET_OS_MAC)
            	timeinfo = localtime(&(directory.st_mtimespec.tv_sec));
			#else
            	timeinfo = localtime(&(directory.st_mtim.tv_sec));
			#endif // TARGET_OS_MAC
			
            strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
            str.append(lastModifications);
            str.append("\t\t");
            if (dirent->d_type == DT_DIR)
                str.append("-");
            else
                str.append(ft_itoa(directory.st_size));
            str.append("\n");
            files.push(str);
        }
        while (!files.empty())
        {
            body.append(files.top());
            files.pop();
        }
    }
    body.append("</pre><hr></body>\n</html>");
    _contentLength = body.length();
    _contentType = "text/html";
    _charset = "utf-8";
    _body = (char*)ft_calloc(_contentLength + 1, sizeof(char));
    ft_strcpy(_body, body.c_str());
}

// ** Check if the autorization mode is on and if the user is authorized to make the request **
void            HTTP::authorization()
{
    int     fd;
    int     ret;
    char    *line;

    if (_config.getAuth_basic(_location).compare("off") != 0)
    {
        if (_socket.getAuthorization().compare(""))
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
                    free(line);
                    line = NULL;
                }
                if (base64_decode(_socket.getAuthorization()).compare(line) == 0)
                    _wwwAuthenticate.assign("OK");
                if (_wwwAuthenticate.compare("OK") != 0)
                {
                    _statusCode = UNAUTHORIZED;
                    _wwwAuthenticate.assign("Basic realm=").append(_config.getAuth_basic(_location));
                }
                free(line);
                line = NULL;
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

void        HTTP::configureErrorFile()
{
    int         ret;
    char        buf[1024 + 1];
    int         fd;
    std::string body;

    _route = _config.getErrorFilesRoot().append("/").append(ft_itoa(_statusCode));
    fd = open(_route.c_str(), O_RDONLY);
    if (fd == -1)
    {
        body.assign("<!DOCTYPE html>\n<html>\n<body>\n\n<h1>");
        body.append(ft_itoa(_statusCode)).append(" ").append(_mapCodes.codes[_statusCode]);
        body.append("</h1>\n\n</body>\n</html>\n");
        _contentType = "text/html";
        _charset = "utf-8";
        _contentLength = body.length();
    }
    else
    {
        body.assign("");
        while ((ret = read(fd, buf, 1024)) > 0)
        {
            buf[ret] = '\0';
            body.append(buf);
        }
        if (ret == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
        else
            close(fd);
        setStat();
        setContentType();
        setContentLength();
        setDate();
    }
    _body = ft_strdup(body.c_str());
}

// ** Create the response socket **
char*         HTTP::getResponse()
{
    std::string response;

    if (_statusCode >= 300)
        configureErrorFile();
    if (_statusCode == 200 && _socket.getMethod().compare("PUT") == 0)
    {
        if (ft_strlen(_date) > 0)
            response.append("Date: ").append(_date).append("\r\n");
    }
    else
    {
        response.append(_config.getHttpVersion());
        response.append(" ");
        response.append(ft_itoa(_statusCode)).append(" ");
        response.append(_mapCodes.codes[_statusCode]).append("\r\n");
        response.append("Server: ").append(_config.getServerSoftware()).append("\r\n"); //TO DO
        if (ft_strlen(_date) > 0)
            response.append("Date: ").append(_date).append("\r\n");

        if (_contentType.length() > 0)
            response.append("Content-Type: ").append(_contentType).append("\r\n");

        if (_contentLength > 0)
            response.append("Content-Length: ").append(ft_itoa(_contentLength)).append("\r\n");
        if (_socket.getMethod().compare("OPTIONS") == 0 || _statusCode == METHOD_NOT_ALLOWED)
        {
            response.append("Allow: ");
            std::vector<std::string>::iterator it;
            std::vector<std::string>::iterator itEnd;
            std::size_t extension;
            std::string str;

            extension = _route.find_last_of('.');
            if (is_good_exe(str.assign(_route).erase(0, extension + 1)))
            {
                it = _config.getCGI_allow(_location).begin();
                itEnd = _config.getCGI_allow(_location).end();
            }
            else
            {
                it = _config.getAllow(_location).begin();
                itEnd = _config.getAllow(_location).end();
            }
            while (it != itEnd)
            {
                response.append(*it).append(" ");
                it++;
            }
            response.append("\r\n");
        }
        else
        {
            if (_charset.length() > 0)
                response.append("Charset: ").append(_charset).append("\r\n");
            if (_statusCode < 300)
            {
                if (ft_strlen(_lastModified) > 0)
                    response.append("Last-Modified: ").append(_lastModified).append("\r\n");
               if (_contentLocation.length() > 0)
                   response.append("Content-Location: ").append(_contentLocation).append("\r\n");
                if (_contentLanguage.length() > 0 && _socket.getMethod().compare("PUT") && _socket.getMethod().compare("DELETE"))
                    response.append("Content-Language: ").append(_contentLanguage).append("\r\n");
            }
            else if (_statusCode == UNAUTHORIZED)
                response.append("WWW-Authenticate: ").append("Basic ").append(_config.getAuth_basic(_location)).append("\r\n");
        }
    }
    response.append("\r\n");
    _responseSize = response.length() + _contentLength;
    _response = (char*)ft_calloc(_responseSize + 1, sizeof(char));
    ft_strcpy(_response, response.c_str());
    std::cerr << "content length: " << _contentLength << std::endl;
    std::cerr << "response length: " << response.length() << std::endl;
    std::cerr << "response size: " << _responseSize << std::endl;
    std::cerr << "_response length: " << ft_strlen(_response) << std::endl;
    if (_socket.getMethod().compare("HEAD"))
        ft_memcat(_response, _body, _contentLength);
    return (_response);
}

int             HTTP::getResponseSize()
{
    return _responseSize;
}
