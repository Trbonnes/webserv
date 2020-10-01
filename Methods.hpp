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
    struct stat                 _stat;
    int                         _response;
    int                         _statusCode;
    std::vector<std::string>    _allow;
    std::string                 _wwwAuthenticate;
    std::string                 _referer;
    std::string                 _lastModified;
    std::string                 _location;
    std::string                 _server;
    std::string                 _contentLanguage;
    int                         _contentLength;
    std::string                 _contentLocation;
    std::string                 _contentType;
    std::string                 _date;
    std::string                 _retryAfter;
    std::string                 _transferEncoding;
    std::string                 _body;
    std::string                 _methodsName[NB_METHODS];
    void                        (*_method[NB_METHODS])(void);


    Methods();

    void            callMethod(int method);
    int             checkAllowMethods(std::string method);

    // GET
    void            get(void);
    int             openFile(std::string root);
    int             setRoot(std::string &root);
    void            setBody(int fd);
    void            setContentType(std::string root);
    std::string     acceptLanguage();
    void            setStat(std::string root);
    void            setContentLength();
    void            setServerName();

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