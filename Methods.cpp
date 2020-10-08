#include "Methods.hpp"

const std::string Methods::_base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

Methods::Methods() :
_config(),
_socket(0),
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
    _methodsName[POST] = "POST"; 
    _methodsName[PUT] = "PUT"; 
    _methodsName[DELETE] = "DELETE"; 
    _methodsName[CONNECT] = "CONNECT"; 
    _methodsName[OPTIONS] = "OPTIONS"; 
    _methodsName[TRACE] = "TRACE"; 
    _methodsName[PATCH] = "PATCH"; 
}

Methods::Methods(std::map<std::string, Config, CompNormalOrder<std::string> > config, Socket &socket) :
_config(config),
_socket(socket),
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
    _methodsName[POST] = "POST"; 
    _methodsName[PUT] = "PUT"; 
    _methodsName[DELETE] = "DELETE"; 
    _methodsName[CONNECT] = "CONNECT"; 
    _methodsName[OPTIONS] = "OPTIONS"; 
    _methodsName[TRACE] = "TRACE"; 
    _methodsName[PATCH] = "PATCH"; 

    int     i;

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

Methods::Methods(Methods &copy)
{
    _socket = copy._socket;
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

Methods::~Methods() {}

Methods     &Methods::operator=(Methods &rhs)
{
    _socket = rhs._socket;
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

void        Methods::callMethod(int method)
{
    if (method == GET)
        get();
    else if (method == HEAD)
        head();
    else if (method == POST)
        post();
    else if (method == PUT)
        put();
    else if (method == CONNECT)
        connect();
    else if (method == DELETE)
        del();
    else if (method == OPTIONS)
        options();
    else if (method == TRACE)
        trace();
    else if (method == PATCH)
        patch();
}

int         Methods::checkAllowMethods(std::string method)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;
    int ret;

    ret = 0;
    itBegin = _config.find(_socket.getHost())->second.getAllow(_uri).begin();
    itEnd = _config.find(_socket.getHost())->second.getAllow(_uri).end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            ret = 1;
        itBegin++;
    }
    _statusCode = METHOD_NOT_ALLOWED;
    return (ret);
}

//** replace URI by the location **
void        Methods::replaceURI()
{
  std::cout << "location: " << _location << std::endl;
  _uri.assign(_config.find(_socket.getHost())->second.getRoot(_location));
}

//** absolute location route for the server **
void        Methods::setLocation()
{
  std::cout << "URI: " << _uri << std::endl;
  if (_config.find(_socket.getHost()) != _config.end())
    std::cout << "FIND!" << std::endl;
  std::cout << "root: " << _config.find(_socket.getHost())->second.getRoot(_uri) << std::endl;
  _location = _config.find(_socket.getHost())->second.getLocation(_uri);
}

//** absolute location route for the user agent **
void        Methods::setContentLocation()
{
    _contentLocation.assign("http://").append(_config.find(_socket.getHost())->second.getServerName(_location)).append(_route);
}

//** Copy file into body string **
void        Methods::setBody(int fd)
{
    int     ret;
    char    buf[1024 + 1];

    while ((ret = read(fd, buf, 1024)) > 0)
    {
        buf[ret] = '\0';
        _body.append(buf);
    }
    if (ret == -1)
        _statusCode = INTERNAL_SERVER_ERROR;
    else
        close(fd);
}

void        Methods::setCharset(void)
{
    _charset = _config.find(_socket.getHost())->second.getCharset(_location);
}


inline bool   Methods::is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

//** Encode password in base64 **
std::string   Methods::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
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
std::string   Methods::base64_decode(std::string const& encoded_string) {
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

int         Methods::getResponse()
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
