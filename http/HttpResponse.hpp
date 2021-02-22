#ifndef HTTP_RESPONSE
# define HTTP_RESPONSE

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
# include <stdio.h> // REMOVE

# include "core/ConfigServer.hpp"
# include "HttpRequest.hpp"
# include "core/utils/utils.hpp"
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
    REDIRECT_STATUS,
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
    X_SECRET,
    NB_METAVARIABLES
};

class   HttpResponse
{
    private:



    HttpRequest*                _request;
    ConfigServer                _config;
    StatusCode                  _mapCodes;
    CGI                         _cgi;
    bool                        _use_cgi;

    // File streams
    int                         _stream_write;
    int                         _stream_read;

    // Utils
    std::string                 _uri;
    std::string                 _route;
    std::string                 _location;
    struct stat                 _stat;
    static const std::string    _base64_chars;
    char                        *_cgi_env[NB_METAVARIABLES + 1];

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
    

    // The http headers as a string buffer
    std::string                 _httpHeaders;


    void            prepapreMethod(std::string method);
    void            callMethod(std::string method);

    int             checkAllowMethods(std::string method);
    int             checkCGImethods(std::string method);
    void            setConfigURI(void);

    
    void            openStreams();
    char*           readBody();



    // Setters
    void            setBody(int fd);
    void            setRoot(void);
    void            setContentType(void);
    std::string     acceptLanguage(void);
    void            setStat(void);
    void            setContentLength(void);
    void            setServerName(void);
    void            setContentLocation(void);
    void            setCharset(void);
    void            setLastModified(void);
    void            setDate(void);
    void            setAutoindex(void);

    void            authorization(void);
    int             openFile(void);
    // AUTHORIZATION BASE64
    static inline
    bool            is_base64(unsigned char c);
    std::string     base64_decode(std::string const& encoded_string);
    std::string     base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

    // CGI
    void            prepare_cgi();
    void            cgi_metaVariables();
    void            cgi_exe();
    void            cgi_parse();
    void            setEnv();
    int             is_good_exe(std::string exe);
    static bool     mypred(char val1, char val2);

    //GET
    void            get(void);
    
    // PUT
    void            put(void);
    
    // DELETE
    void            del(void);

    // ERRORS
    void            configureErrorFile(void);

    public:
    HttpResponse();
    HttpResponse(HttpRequest *socket, ConfigServer *config);
    HttpResponse(HttpResponse &copy);
    ~HttpResponse();

    HttpResponse            &operator=(HttpResponse &rhs);

    //Getters
    std::string&    getTransferEncoding();

    // RESPONSE
    void            read_cgi_response();
    int             write_cgi_request();
    bool            use_cgi();
    void            processResponse();
    void            setFirstHeadersResponse(std::string &response);
    void            setAllowMethodsResponse(std::string &response);
    void            setOtherHeaders(std::string &response);
};

#endif