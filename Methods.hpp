#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <dirent.h>
# include <string>
# include <fcntl.h>
# include <algorithm>
# include <stack>
# include "TEST/Socket.hpp"
# include "TEST/Config.hpp"
# include "utils/utils.hpp"
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
    static const std::string    _base64_chars;

    // headers

    int                         _statusCode;
    std::vector<std::string>    _allow;
    std::string                 _wwwAuthenticate;
    std::string                 _referer;
    char                        _lastModified[100];
    char                        _date[100];
    std::string                 _server;
    std::string                 _contentLanguage;
    int                         _contentLength;
    std::string                 _contentLocation;
    std::string                 _contentType;
    std::string                 _charset;
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
    void            setCharset(void);
    void            authorization(void);
    void            setLastModified(void);
    void            setDate(void);
    void            setAutoindex(void);

    static inline
    bool            is_base64(unsigned char c);
    std::string     base64_decode(std::string const& encoded_string);
    std::string     base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

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
    Methods(Methods &copy);
    ~Methods();

    Methods         &operator=(Methods &rhs);
    int             getResponse();
};

#endif