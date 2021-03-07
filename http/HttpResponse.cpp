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
_is_done(false),
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

    size_t      extension;
    std::string str;

    extension = _route.find_last_of('.');
    // If it's a CGI request we must fork and prepare the stream in and out
    std::cout << "Are we in cgi ?" << (is_good_exe(str.assign(_route).erase(0, extension + 1)) && checkCGImethods(_request->getMethod())) << std::endl;
    std::cout << "With " << is_good_exe(str.assign(_route).erase(0, extension + 1)) << " and " << checkCGImethods(_request->getMethod()) << std::endl;
    
    if (is_good_exe(str.assign(_route).erase(0, extension + 1)) && checkCGImethods(_request->getMethod()))
    {
        cgi();
    }
    // If it's not CGI we got to open read streams or write streams
    else if (checkAllowMethods(_request->getMethod()))
    {
        std::string& method = _request->getMethod();

        // TO DO check if if authorized in a return boolean way
        authorization();
        if (method.compare("GET") == 0)
            get(true);
        else if (method.compare("POST") == 0)
            post();
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

    itBegin = _config.getAllow(_location).begin();
    itEnd = _config.getAllow(_location).end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            return 1;
        itBegin++;
    }
    return 0;
}


// ** Set the root with language and location directory if needed **
void         HttpResponse::setRoute()
{
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;
    struct stat file;
    std::string str;
    int ret;

    std::string root = _config.getRoot(_location);  
    std::string alias = _config.getAlias(_location);


    Log::debug(_route);
    //** Relative path **
    if (alias.length() > 0)
        _route.assign(alias).append("/");
    else
        _route.assign(root);
    ret = stat(_route.c_str(), &file);
    // if is file  exists or put request we're done
    if ((ret == 0 && S_ISREG(file.st_mode)) || _request->getMethod() == "PUT")
        return;
    // accpet lagnguage
    _route.append(acceptLanguage());
    _route.append(str.assign(_request->getRequestURI()).erase(0, _location.length()));
    // ** Else, add index if it is not a put or delete request **
    itIndexBegin = _config.getIndex(_location).begin();
    itIndexEnd = _config.getIndex(_location).end();
    stat(_route.c_str(), &file);
    str.assign(_route);
    while (itIndexBegin != itIndexEnd && !S_ISREG(file.st_mode))
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
            response.append("Transfer-Encoding: ").append(_transferEncoding);
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
