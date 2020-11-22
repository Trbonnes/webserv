#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <strings.h>
# include <dirent.h>
# include <string>
# include <fcntl.h>
# include <algorithm>
# include <stack>

# include "Socket.hpp"
# include "../core/ConfigServer.hpp"
# include "utils/utils.hpp"
# include "statusCodes.hpp"
# include "CGI.hpp"

# define SIDE_OUT 0
# define SIDE_IN 1

enum methods
{
    GET,
    HEAD,
    NB_METHODS
};

enum cgi_variables
{
    AUTH_TYPE,
    CONTENT_LENGTH,
    CONTENT_TYPE,
    GATEWAY_INTERFACE,
    PATH_INFO,
    PATH_TRANSLATED,
    QUERY_STRING,
    REMOTE_ADDR,
    REMOTE_IDENT,
    REMOTE_USER,
    REQUEST_METHOD,
    REQUEST_URI,
    SCRIPT_NAME,
    SERVER_NAME,
    SERVER_PORT,
    SERVER_PROTOCOL,
    SERVER_SOFTWARE,
    NB_METAVARIABLES
};

class   HTTP
{
    private:

    Socket                      _socket;
    ConfigServer                _config;
    StatusCode                  _mapCodes;
    CGI                         _cgi;
    std::string                 _uri;
    std::string                 _route;
    std::string                 _location;
    struct stat                 _stat;
    static const std::string    _base64_chars;
    char                        *_cgi_env[18];

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

    std::string                 _OLDbody; // A MODIFIER LE CGI
    char*                       _body;
    char*                       _response;
    int                         _responseSize;

    HTTP();

    void            callMethod(std::string method);
    int             checkAllowMethods(std::string method);
    int             checkCGImethods(std::string method);
    void            setConfigURI(void);
    void            setLocation(void);
    void            replaceURI(void);

    // GET
    void            get(void);
    int             openFile(void);
    void            setRoot(void);
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

    void            cgi_metaVariables();
    void            cgi_exe();
    void            setEnv();
    int             is_good_exe(std::string exe);
    static bool     mypred(char val1, char val2);

    // HEAD
    void            head(void);

    // PUT
    void            put(void);
    
    // DELETE
    void            del(void);

    // RESPONSE
    void            configureErrorFile(void);

    // ERRORS

    int             checkRequestErrors(void);

    public:
    HTTP(Socket *socket, ConfigServer &config);
    HTTP(HTTP &copy);
    ~HTTP();

    HTTP            &operator=(HTTP &rhs);
    char*           getResponse();
    int             getResponseSize();
};

#endif