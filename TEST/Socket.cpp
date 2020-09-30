#include "Socket.hpp"

Socket::Socket() :
_fd(0),
_method("GET"),
_requestURI("http://localhost/index.html"),
_httpVersion("HTTP/1.1"),
_contentLength(""),
_contentLocation(""),
_contentType(""),
_date(""),
_transferEncoding(""),
_acceptCharset(""),
_acceptLanguage("en-US,en;q="".9"),
_authorization(""),
_host("localhost"),
_userAgent("Mozilla/5."" (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/8""."".3987.149 Safari/537.36"),
_body("") {
}

Socket::Socket(int fd) :
_fd(fd),
_method(""),
_requestURI(""),
_httpVersion(""),
_contentLength(""),
_contentLocation(""),
_contentType(""),
_date(""),
_transferEncoding(""),
_acceptCharset(""),
_acceptLanguage(""),
_authorization(""),
_host(""),
_userAgent(""),
_body("") {
    parser();
}

Socket::Socket(Socket const &copy)
{
    _fd = copy._fd;
    _method = copy._method;
    _requestURI = copy._requestURI;
    _httpVersion = copy._httpVersion;
    _contentLength = copy._contentLength;
    _contentLocation = copy._contentLocation;
    _contentType = copy._contentType;
    _date = copy._date;
    _transferEncoding = copy._transferEncoding;
    _acceptCharset = copy._acceptCharset;
    _acceptLanguage = copy._acceptLanguage;
    _authorization = copy._authorization;
    _host = copy._host;
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
    _contentLength = rhs._contentLength;
    _contentLocation = rhs._contentLocation;
    _contentType = rhs._contentType;
    _date = rhs._date;
    _transferEncoding = rhs._transferEncoding;
    _acceptCharset = rhs._acceptCharset;
    _acceptLanguage = rhs._acceptLanguage;
    _authorization = rhs._authorization;
    _host = rhs._host;
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

std::string     Socket::getTransferEncoding()
{
    return _transferEncoding;
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

std::string     Socket::getUserAgent()
{
    return _userAgent;
}

std::string     Socket::getBody()
{
    return _body;
}
