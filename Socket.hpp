#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>

class   Socket
{
    private:

    // SOCKET

    int             _fd;

    // REQUEST LINE

    std::string     _method;
    std::string     _requestURI;
    std::string     _httpVersion;

    // HEADERS

    std::string     _allow;
    std::string     _contentLanguage;
    std::string     _contentLength;
    std::string     _contentLocation;
    std::string     _contentType;
    std::string     _date;
    std::string     _lastModified;
    std::string     _location;
    std::string     _server;
    std::string     _transferEncoding;
    std::string     _wwwAuthenticate;
    std::string     _acceptCharset;
    std::string     _acceptLanguage;
    std::string     _authorization;
    std::string     _host;
    std::string     _referer;
    std::string     _userAgent;

    // BODY

    std::string     _body;

    // PRIVATE FUNCTIONS

    Socket();
    void            parser(void);

    public:
    Socket(int fd);
    Socket(Socket const &copy);
    ~Socket();

    Socket          &operator=(Socket const &rhs);
    int             getFd();
    std::string     getMethod();
    std::string     getRequestURI();
    std::string     gethttpVersion();
    std::string     getAllow();
    std::string     getContentLanguage();
    std::string     getContentLenght();
    std::string     getContentLocation();
    std::string     getContentType();
    std::string     getDate();
    std::string     getLastModified();
    std::string     getLocation();
    std::string     getServer();
    std::string     getTransferEncoding();
    std::string     getWWWAuthenticate();
    std::string     getAcceptCharset();
    std::string     getAcceptLanguage();
    std::string     getAuthorization();
    std::string     getHost();
    std::string     getReferer();
    std::string     getUserAgent();
    std::string     getBody();
};

#endif