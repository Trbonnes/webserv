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

class   HttpResponse
{
    private:


    HttpRequest*                _request;
    ConfigServer                _config;
    StatusCode                  _mapCodes;
    CGI                         _cgi;
    bool                        _use_cgi;
    bool                        _is_done;
    bool                        _ignore_body;


    // File streams
    int                         _stream_write;
    int                         _stream_read;
    

    // Utils
    std::string                 _uri;
    std::string                 _route;
    std::string                 _location;
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


    std::string*                _headers;
    std::string*                _body;


    int             checkAllowMethods(std::string method);
    int             checkCGImethods(std::string method);
    void            setConfigURI(void);

    void            init();
    void            get(bool);
    void            del();
    void            put();
    void            post();
    void            head();
    void            options();
    void            cgi();
    void            error();


    void            autoIndex();


    // Setters
    void            setRoute(void);
    void            setContentType(void);
    std::string     acceptLanguage(void);
    void            setServerName(void);
    void            setContentLocation(void);
    void            setCharset(void);
    void            setLastModified(struct stat*);
    void            setDate(void);
    

    void            authorization(void);
    int             openFile(void);
    // AUTHORIZATION BASE64
    static inline
    bool            is_base64(unsigned char c);
    std::string     base64_decode(std::string const& encoded_string);
    std::string     base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

    // CGI
    void            cgi_metaVariables();
    void            cgi_exe();
    void            cgi_parse();
    void            setEnv();
    int             is_good_exe(std::string exe);
    static bool     mypred(char val1, char val2);

    // ERRORS
    void            configureErrorFile(std::string&);

    public:
    HttpResponse();
    HttpResponse(HttpRequest *socket, ConfigServer *config);
    HttpResponse(HttpResponse &copy);
    ~HttpResponse();

    HttpResponse            &operator=(HttpResponse &rhs);


    //Getters
    std::string&    getTransferEncoding();
    int             getStreamWrite();
    int             getStreamRead();


    std::string*    getHeaders();
    std::string*    getBody();
    bool            use_cgi();
    void            setFirstHeadersResponse(std::string &response);
    void            setAllowMethodsResponse(std::string &response);
    void            setOtherHeaders(std::string &response);
};

#endif