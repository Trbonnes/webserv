#include "Socket.hpp"

Socket::Socket() :
_fd(0),
_method(0),
_requestURI(0),
_httpVersion(0),
_allow(0),
_contentLanguage(0),
_contentLength(0),
_contentLocation(0),
_contentType(0),
_date(0),
_lastModified(0),
_location(0),
_server(0),
_transferEncoding(0),
_wwwAuthenticate(0),
_acceptCharset(0),
_acceptLanguage(0),
_authorization(0),
_host(0),
_referer(0),
_userAgent(0),
_body(0) {
}

Socket::Socket(int fd) :
_fd(fd),
_method(0),
_requestURI(0),
_httpVersion(0),
_allow(0),
_contentLanguage(0),
_contentLength(0),
_contentLocation(0),
_contentType(0),
_date(0),
_lastModified(0),
_location(0),
_server(0),
_transferEncoding(0),
_wwwAuthenticate(0),
_acceptCharset(0),
_acceptLanguage(0),
_authorization(0),
_host(0),
_referer(0),
_userAgent(0),
_body(0) {
    parser();
}

Socket::Socket(Socket const &copy)
{
    _fd = copy._fd;
    _method = copy._method;
    _requestURI = copy._requestURI;
    _httpVersion = copy._httpVersion;
    _allow = copy._allow;
    _contentLanguage = copy._contentLanguage;
    _contentLength = copy._contentLength;
    _contentLocation = copy._contentLocation;
    _contentType = copy._contentType;
    _date = copy._date;
    _lastModified = copy._lastModified;
    _location = copy._location;
    _transferEncoding = copy._transferEncoding;
    _wwwAuthenticate = copy._wwwAuthenticate;
    _acceptCharset = copy._acceptCharset;
    _acceptLanguage = copy._acceptLanguage;
    _authorization = copy._authorization;
    _host = copy._host;
    _referer = copy._referer;
    _userAgent = copy._userAgent;
    _body = copy._body;
}

Socket::~Socket() {}

Socket      &Socket::operator=(Socket const &rhs)
{
    _fd = rhs._fd;
    _method = rhs._method;
    _requestURI = rhs._requestURI;
    _httpVersion = rhs._httpVersion;
    _allow = rhs._allow;
    _contentLanguage = rhs._contentLanguage;
    _contentLength = rhs._contentLength;
    _contentLocation = rhs._contentLocation;
    _contentType = rhs._contentType;
    _date = rhs._date;
    _lastModified = rhs._lastModified;
    _location = rhs._location;
    _transferEncoding = rhs._transferEncoding;
    _wwwAuthenticate = rhs._wwwAuthenticate;
    _acceptCharset = rhs._acceptCharset;
    _acceptLanguage = rhs._acceptLanguage;
    _authorization = rhs._authorization;
    _host = rhs._host;
    _referer = rhs._referer;
    _userAgent = rhs._userAgent;
    _body = rhs._body;
    return *this;
}

void        Socket::parser(void)
{
    return ;
}

std::string     Socket::getMethod()
{
    return _method;
}

std::string     Socket::getRequestURI()
{
    return _requestURI;
}

std::string     Socket::gethttpVersion()
{
    return _httpVersion;
}

std::string     Socket::getAllow()
{
    return _allow;
}

std::string     Socket::getContentLanguage()
{
    return _contentLanguage;
}

std::string     Socket::getContentLenght()
{
    return _contentLength;
}

std::string     Socket::getContentLocation()
{
    return _contentLocation;
}

std::string     Socket::getContentType()
{
    return _contentType;
}

std::string     Socket::getDate()
{
    return _date;
}

std::string     Socket::getLastModified()
{
    return _lastModified;
}

std::string     Socket::getLocation()
{
    return _location;
}

std::string     Socket::getServer()
{
    return _server;
}

std::string     Socket::getTransferEncoding()
{
    return _transferEncoding;
}

std::string     Socket::getWWWAuthenticate()
{
    return _wwwAuthenticate;
}

std::string     Socket::getAcceptCharset()
{
    return _acceptCharset;
}

std::string     Socket::getAcceptLanguage()
{
    return _acceptLanguage;
}

std::string     Socket::getAuthorization()
{
    return _authorization;
}

std::string     Socket::getHost()
{
    return _host;
}

std::string     Socket::getReferer()
{
    return _referer;
}

std::string     Socket::getUserAgent()
{
    return _userAgent;
}

std::string     Socket::getBody()
{
    return _body;
}
