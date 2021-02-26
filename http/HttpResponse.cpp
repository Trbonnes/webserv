#include "HttpResponse.hpp"

const std::string HttpResponse::_base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

HttpResponse::HttpResponse() :
_request(NULL),
_config(),
_mapCodes(),
_cgi(),
_use_cgi(false),
_location(""),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_date(""),
_server(""),
_contentLanguage(""),
_contentLength(-1),
_contentLocation(""),
_contentType(""),
_charset(""),
_retryAfter(""),
_transferEncoding("")
{
}

HttpResponse::HttpResponse(HttpRequest *req, ConfigServer *config) :
_request(req),
_config(*config),
_mapCodes(),
_cgi(),
_use_cgi(false),
_stream_write(-1),
_stream_read(-1),
_location(""),
_statusCode(OK),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_date(""),
_server(""),
_contentLanguage(""),
_contentLength(-1),
_contentLocation(""),
_contentType(""),
_charset(""),
_retryAfter(""),
_transferEncoding("")
{
    _body = NULL;
    _headers = NULL;
    ft_bzero(_cgi_env, sizeof(NB_METAVARIABLES + 1));
    init(); // TO DO should i just copy the code of init in here ? 
}

HttpResponse::HttpResponse(HttpResponse &copy)
{
    _request = copy._request;
    _config = copy._config;
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
}

HttpResponse::~HttpResponse()
{
    int     i;

    i = 0;
    while (i < NB_METAVARIABLES)
    {
        if (_cgi_env[i])
            free(_cgi_env[i]);
        _cgi_env[i] = NULL;
        i++;
    }
}

HttpResponse     &HttpResponse::operator=(HttpResponse &rhs)
{
    _request = rhs._request;
    _config = rhs._config;
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
    return *this;
}

// This method opens the read/write streams needed to pursue the request 
void            HttpResponse::init()
{
    _statusCode = OK;
    ft_bzero(_cgi_env, sizeof(char*) * NB_METAVARIABLES + 1);
    _uri = _request->getRequestURI();
    // Absolute location route for the server
    _location = _config.getLocation(_uri);
    // Check if length is given
    if (_request->getBody().length() > 0 && _request->getContentLength() == 0 && _request->getTransferEncoding().length() == 0)
        _statusCode = LENGTH_REQUIRED;
    // Check to see if the request body is too large for the 
    else if (_config.getClientBodySize(_location) != -1 && _request->getContentLength() > _config.getClientBodySize(_location))
        _statusCode = REQUEST_ENTITY_TOO_LARGE;
    if (_request->getMethod().compare("OPTIONS") == 0)
        _statusCode = NO_CONTENT;
    
    // In all cases we set the date
    setDate();
    // set the route of the ressource
    setRoute();
    // TO DO set server

    size_t      extension;
    std::string str;

    extension = _route.find_last_of('.');
    // If it's a CGI request we must fork and prepare the stream in and out
    if (is_good_exe(str.assign(_route).erase(0, extension + 1)) && checkCGImethods(_request->getMethod()))
        cgi();
    // If it's not CGI we got to open read streams or write streams
    else if (checkAllowMethods(_request->getMethod()))
    {
        std::string& method = _request->getMethod();
        

        // TO DO check if if authorized in a return boolean way
        authorization();

        if (method.compare("GET") == 0)
            get(true);
        else if (method.compare("HEAD") == 0)
            get(false); // the fasle here means to not include the body in the response
        else if (method.compare("PUT") == 0)
            put();
        else if (method.compare("DELETE") == 0)
            del();
        else if (method.compare("OPTIONS") == 0)
            options();
    }
    // If else the method is not allowed
    else
        _statusCode = METHOD_NOT_ALLOWED;

    // At the end. check if no error was met
    if (_statusCode >= 300)
        error();

    _headers = new std::string();
    // Headers processing
    // set headers
    setFirstHeadersResponse(*_headers);
    if (_request->getMethod().compare("OPTIONS") == 0 || _statusCode == METHOD_NOT_ALLOWED)
        setAllowMethodsResponse(*_headers);
    else
        setOtherHeaders(*_headers);
    // end of headers
    _headers->append("\r\n");

    if (_request->getMethod().compare("HEAD") == 0 && _body)
    {
        delete _body;
        _body = 0;
    }
}

//** Check if the method is authorized for the non CGI locations **
int         HttpResponse::checkAllowMethods(std::string method)
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

// ** Set the root with language and location directory if needed **
void         HttpResponse::setRoute()
{
    int         fd;
    int         find;
    std::string str;
    struct stat file;
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;

    ft_bzero(&file, sizeof(file));
    if (_uri.compare(0, 4, "http") == 0)
    {
        //** Absolute path **
        find = _route.append(_request->getRequestURI()).find(_request->getHost());
        _route.erase(0, find + _config.getServerName()[0].length());
        _route.insert(0, _config.getRoot(_location));
        _route.insert(_config.getRoot(_location).length(), acceptLanguage());
    }
    else
    {
        //** Replace URI by the location **
        _uri.assign(_config.getRoot(_location));
        
        //** Relative path **
        if (_config.getAlias(_location).length() > 0)
            _route.assign(_config.getAlias(_location)).append("/");
        else
        {
            _route.assign(_config.getRoot(_location));
            _route.append(_request->getRequestURI());
        }
        stat(_route.c_str(), &file);

        // ** If file exist or put request, return **
        if (((S_ISREG(file.st_mode) && (fd = open(_route.c_str(), O_RDONLY)) != -1)) || _request->getMethod().compare("PUT") == 0
            || (((file.st_mode & S_IFMT) == S_IFDIR) && _request->getMethod().compare("DELETE") == 0))
        {
            close(fd);
            return ;
        }

        // ** Else, add the language **
        _route.assign(_config.getRoot(_location));
        if (_config.getAlias(_location).length() > 0)
            _route.assign(_config.getAlias(_location)).append("/");
        _route.append(acceptLanguage());
        _route.append(str.assign(_request->getRequestURI()).erase(0, _location.length()));
        
        stat(_route.c_str(), &file);
        // ** If file exist or delete request, return **
        if ((((file.st_mode & S_IFMT) == S_IFREG && (fd = open(_route.c_str(), O_RDONLY)) != -1))
        || _request->getMethod().compare("DELETE") == 0)
        {
            close(fd);
            return ;
        }

        // ** Else, add index if it is not a put or delete request **
        itIndexBegin = _config.getIndex(_location).begin();
        itIndexEnd = _config.getIndex(_location).end();
        stat(_route.c_str(), &file);
        str.assign(_route);
        while (itIndexBegin != itIndexEnd &&
        ((file.st_mode & S_IFMT) != S_IFREG && (fd = open(_route.c_str(), O_RDONLY)) != -1))
        {
            str.assign(_route);
            if (str.at(str.length() - 1) != '/')
                str.append("/");
            str.append(*itIndexBegin);
            stat(str.c_str(), &file);
            itIndexBegin++;
            close(fd);
        }
        _route.assign(str);
    }
    if ((fd = open(_route.c_str(), O_RDONLY)) == -1)
        _route = _request->getRequestURI();
    else
        close(fd);
    return ;
}

// ** Check if the autorization mode is on and if the user is authorized to make the request **
void            HttpResponse::authorization()
{
    int     fd;
    int     ret;
    char    *line;
    size_t  length;

    if (_config.getAuth_basic(_location).compare("") != 0)
    {
        if (_request->getAuthorization().compare("") == 0)
            _statusCode = UNAUTHORIZED;
        else
        {
            if ((fd = open(_config.getAuth_basic_user_file(_location).c_str(), O_RDONLY)) >= 0)
            {
                while ((ret = get_next_line(fd, &line)) > 0)
                {
                    length = _request->getAuthorization().length();
                    if (base64_decode(_request->getAuthorization().substr(6, length)).compare(line) == 0)
                    {
                        _wwwAuthenticate.assign("OK");
                        break ;
                    }
                    free(line);
                    line = NULL;
                }
                if (base64_decode(_request->getAuthorization().substr(6, length)).compare(line) == 0)
                    _wwwAuthenticate.assign("OK");
                if (_wwwAuthenticate.compare("OK") != 0)
                    _statusCode = UNAUTHORIZED;
                free(line);
                line = NULL;
                close(fd);
            }
        }
    }
    else
        _wwwAuthenticate.assign("OK");
}

inline bool   HttpResponse::is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

//** Encode password in base64 **
std::string   HttpResponse::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
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
std::string   HttpResponse::base64_decode(std::string const& encoded_string) {
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


void     HttpResponse::setFirstHeadersResponse(std::string &response)
{
    response.append(_config.getHttpVersion());
    response.append(" ");
    char *tmp = ft_itoa(_statusCode);
    response.append(tmp).append(" ");
    free(tmp);
    response.append(_mapCodes.codes[_statusCode]).append("\r\n");
    response.append("Server: ").append(_config.getServerSoftware()).append("\r\n");
    if (ft_strlen(_date) > 0)
        response.append("Date: ").append(_date).append("\r\n");
    if (_contentType.length() > 0)
        response.append("Content-Type: ").append(_contentType).append("\r\n");
    if (_contentLength >= 0)
    {
        tmp = ft_itoa(_contentLength);
        response.append("Content-Length: ").append(tmp).append("\r\n");
        free(tmp);
    }
}

void            HttpResponse::setAllowMethodsResponse(std::string &response)
{
        std::vector<std::string>::iterator it;
        std::vector<std::string>::iterator itEnd;
        std::size_t extension;
        std::string str;

        response.append("Allow: ");
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

void            HttpResponse::setOtherHeaders(std::string &response)
{
    if (_charset.length() > 0)
        response.append("Charset: ").append(_charset).append("\r\n");
    if (_statusCode < 300)
    {
        if (ft_strlen(_lastModified) > 0)
            response.append("Last-Modified: ").append(_lastModified).append("\r\n");
        if (_contentLocation.length() > 0)
            response.append("Content-Location: ").append(_contentLocation).append("\r\n");
        if (_contentLanguage.length() > 0 && _request->getMethod().compare("PUT") && _request->getMethod().compare("DELETE"))
            response.append("Content-Language: ").append(_contentLanguage).append("\r\n");
        if (_transferEncoding.length() > 0)
            response.append("Tranfer-Encoding: ").append(_transferEncoding);
    }
    else if (_statusCode == UNAUTHORIZED)
        response.append("WWW-Authenticate: ").append("Basic realm=").append(_config.getAuth_basic(_location)).append("\r\n");
}

std::string&    HttpResponse::getTransferEncoding()
{
    return _transferEncoding;
}

int             HttpResponse::getStreamWrite()
{
    return _stream_write;
}

int             HttpResponse::getStreamRead()
{
    return _stream_read;
}

std::string*    HttpResponse::getHeaders()
{
    return _headers;
}

std::string*    HttpResponse::getBody()
{
    return _body;
}

std::string     HttpResponse::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;
    int r;

    if (!_config.getLanguage(_location).empty())
    {
        str.assign("/");
        itClientBegin = _request->getAcceptLanguage().begin();
        itClientEnd = _request->getAcceptLanguage().end();
        itServerEnd = _config.getLanguage(_location).end();
        while (itClientBegin != itClientEnd)
        {
            itServer = std::find(_config.getLanguage(_location).begin(), _config.getLanguage(_location).end(), *itClientBegin);
            if (itServer != itServerEnd)
            {
                _contentLanguage = *itServer;
                str.assign(*itServer);
                str.append("/");
                r = stat(trydir.assign(_route).append(str).c_str(), &dir);
                if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);
                else
                    _contentLanguage.assign("");
            }
            itClientBegin++;
        }
        _contentLanguage = *(_config.getLanguage(_location).begin());
        str.append(*(_config.getLanguage(_uri).begin()));
        r = stat(trydir.assign(_route).append(str).c_str(), &dir);
        if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
        else
            return (_contentLanguage.assign(""));
    }
    return ("");
}


// ** Create the default html page when file is not found and autoindex is on **
void            HttpResponse::autoIndex()
{
    std::string             str;
    struct stat             directory;
    DIR                     *dir;
    struct dirent           *dirent;
    struct tm               *timeinfo;
    char                    lastModifications[100];
    std::stack<std::string> files;

    dir = opendir(_route.c_str());
    // if there's an se terror to internal server error
    if (dir == NULL)
        _statusCode = INTERNAL_SERVER_ERROR;
    // else create the autoindex body
    else
    {
        _body = new std::string();
        _body->append("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
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
			#ifdef __linux__
            	timeinfo = localtime(&(directory.st_mtim.tv_sec));
			#else
            	timeinfo = localtime(&(directory.st_mtimespec.tv_sec));
			#endif // TARGET_OS_MAC
			
            strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
            str.append(lastModifications);
            str.append("\t\t");
            if (dirent->d_type == DT_DIR)
                str.append("-");
            else
            {
                char *dirSize = ft_itoa(directory.st_size);
                str.append(dirSize);
                free(dirSize);
            }
            str.append("\n");
            files.push(str);
        }
        while (!files.empty())
        {
            _body->append(files.top());
            files.pop();
        }
        _contentLength = _body->length();
        _contentType = "text/html";
        _charset = "utf-8";
        closedir(dir);
        _body->append("</pre><hr></body>\n</html>");
    }
}