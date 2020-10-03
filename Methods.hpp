#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string>
# include <fcntl.h>
# include <algorithm>
# include "TEST/Socket.hpp"
# include "TEST/Config.hpp"
# include "statusCodes.hpp"

enum
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    NB_METHODS
};

class   Methods : public Config
{
    private:

    Socket                      _socket;
    std::string                 _uri;
    std::string                 _route;
    std::string                 _location;
    struct stat                 _stat;
    int                         _response;

    // headers

    int                         _statusCode;
    std::vector<std::string>    _allow;
    std::string                 _wwwAuthenticate;
    std::string                 _referer;
    std::string                 _lastModified;
    std::string                 _server;
    std::string                 _contentLanguage;
    int                         _contentLength;
    std::string                 _contentLocation;
    std::string                 _contentType;
    std::string                 _date;
    std::string                 _retryAfter;
    std::string                 _transferEncoding;
    
    // body

    std::string                 _body;
    
    // methods

    std::string                 _methodsName[NB_METHODS];
    void                        (*_method[NB_METHODS])(void);

    Methods();

    void            callMethod(int method);
    int             checkAllowMethods(std::string method);
    void            setConfigURI(void);
    void            setLocation(void);
    void            replaceURI(void);

    // GET
    void            get(void);
    int             openFile(void);
    int             setRoot(void);
    void            setBody(int fd);
    void            setContentType(void);
    std::string     acceptLanguage(void);
    void            setStat(void);
    void            setContentLength(void);
    void            setServerName(void);
    void            setContentLocation(void);

    // HEAD
    void            head(void);

    // POST
    void            post(void);

    // PUT
    void            put(void);

    // DEL
    void            del(void);

    // CONNECT
    void            connect(void);

    // OPTIONS
    void            options(void);

    // TRACE
    void            trace(void);

    // PATCH
    void            patch(void);

    
    public:
    Methods(Socket &socket);
    Methods(Methods const &copy);
    ~Methods();

    Methods         &operator=(Methods const &rhs);
    int             getResponse();
};

#endif