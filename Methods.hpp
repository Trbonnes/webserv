#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include "Socket.hpp"
# include "Config.hpp"

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

    Socket          _socket;
    int             _response;
    int             _error;
    std::string     _allow;
    std::string     _wwwAuthenticate;
    std::string     _referer;
    std::string     _lastModified;
    std::string     _location;
    std::string     _server;
    std::string     _contentLanguage;
    std::string     _contentLength;
    std::string     _contentLocation;
    std::string     _contentType;
    std::string     _date;
    std::string     _retryAfter;
    std::string     _transferEncoding;
    std::string     _body;
    std::string     _methodsName[NB_METHODS];
    void            (*_method[NB_METHODS])(void);


    Methods();

    void            callMethod(int method);

    void            get(void);
    void            head(void);
    void            post(void);
    void            put(void);
    void            del(void);
    void            connect(void);
    void            options(void);
    void            trace(void);
    void            patch(void);

    
    public:
    Methods(Socket &socket);
    Methods(Methods const &copy);
    ~Methods();

    Methods         &operator=(Methods const &rhs);
    int             getResponse();
};

#endif