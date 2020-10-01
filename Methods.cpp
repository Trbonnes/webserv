#include "Methods.hpp"

Methods::Methods() :
_socket(0),
_stat(),
_response(0),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_contentLength(0),
_contentLocation(""),
_contentType(""),
_date(""),
_retryAfter(""),
_transferEncoding(""),
_body("") {
    _methodsName[GET] = "GET"; 
    _methodsName[HEAD] = "HEAD"; 
    _methodsName[POST] = "POST"; 
    _methodsName[PUT] = "PUT"; 
    _methodsName[DELETE] = "DELETE"; 
    _methodsName[CONNECT] = "CONNECT"; 
    _methodsName[OPTIONS] = "OPTIONS"; 
    _methodsName[TRACE] = "TRACE"; 
    _methodsName[PATCH] = "PATCH"; 
}

Methods::Methods(Socket &socket) :
_socket(socket),
_stat(),
_response(0),
_allow(0),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_contentLength(0),
_contentLocation(""),
_contentType(""),
_date(""),
_retryAfter(""),
_transferEncoding(""),
_body("")
{
    _methodsName[GET] = "GET"; 
    _methodsName[HEAD] = "HEAD"; 
    _methodsName[POST] = "POST"; 
    _methodsName[PUT] = "PUT"; 
    _methodsName[DELETE] = "DELETE"; 
    _methodsName[CONNECT] = "CONNECT"; 
    _methodsName[OPTIONS] = "OPTIONS"; 
    _methodsName[TRACE] = "TRACE"; 
    _methodsName[PATCH] = "PATCH"; 

    int     i;

    i = 0;
    while (i < NB_METHODS)
    {
        if (_socket.getMethod().compare(_methodsName[i]) == 0)
        {
            if (checkAllowMethods(_methodsName[i]))
            {
                callMethod(i);
                return ;
            }
            else
                return ;
        }
        i++;
    }
    _statusCode = BAD_REQUEST;
}

Methods::Methods(Methods const &copy)
{
    _socket = copy._socket;
    _response = copy._response;
    _stat = copy._stat;
    _allow = copy._allow;
    _wwwAuthenticate = copy._wwwAuthenticate;
    _referer = copy._referer;
    _lastModified = copy._lastModified;
    _location = copy._location;
    _server = copy._server;
    _contentLanguage = copy._contentLanguage;
    _contentLength = copy._contentLength;
    _contentLocation = copy._contentLocation;
    _contentType = copy._contentType;
    _date = copy._date;
    _retryAfter = copy._retryAfter;
    _transferEncoding = copy._transferEncoding;
    _body = copy._body;
    for (int i = 0; i < NB_METHODS; i++)
    {
        _methodsName[i].assign(copy._methodsName[i]);
        _method[i] = copy._method[i];
    }
}

Methods::~Methods() {}

Methods     &Methods::operator=(Methods const &rhs)
{
    _socket = rhs._socket;
    _response = rhs._response;
    _stat = rhs._stat;
    _allow = rhs._allow;
    _wwwAuthenticate = rhs._wwwAuthenticate;
    _referer = rhs._referer;
    _lastModified = rhs._lastModified;
    _location = rhs._location;
    _server = rhs._server;
    _contentLanguage = rhs._contentLanguage;
    _contentLength = rhs._contentLength;
    _contentLocation = rhs._contentLocation;
    _contentType = rhs._contentType;
    _date = rhs._date;
    _retryAfter = rhs._retryAfter;
    _transferEncoding = rhs._transferEncoding;
    _body = rhs._body;
    for (int i = 0; i < NB_METHODS; i++)
    {
        _methodsName[i].assign(rhs._methodsName[i]);
        _method[i] = rhs._method[i];
    }
    return *this;
}

void        Methods::callMethod(int method)
{
    if (method == GET)
        get();
    else if (method == HEAD)
        head();
    else if (method == POST)
        post();
    else if (method == PUT)
        put();
    else if (method == CONNECT)
        connect();
    else if (method == DELETE)
        del();
    else if (method == OPTIONS)
        options();
    else if (method == TRACE)
        trace();
    else if (method == PATCH)
        patch();
}

int         Methods::checkAllowMethods(std::string method)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;
    int ret;

    ret = 0;
    itBegin = getDefaultAllow().begin();
    itEnd = getDefaultAllow().end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            ret = 1;
        itBegin++;
    }
    _statusCode = 405;
    return (ret);
}

void        Methods::head()
{
    return ;
}

void        Methods::post()
{
    return ;
}

void        Methods::put()
{
    return ;
}

void        Methods::del()
{
    return ;
}

void        Methods::connect()
{
    return ;
}

void        Methods::options()
{
    return ;
}

void        Methods::trace()
{
    return ;
}

void        Methods::patch()
{
    return ;
}

int         Methods::getResponse()
{
    std::vector<std::string>::iterator it;

    std::cout << "STATUS CODE: " << _statusCode << std::endl << std::endl;

    it = _allow.begin();
    std::cout << "ALLOW: ";
    while (it != _allow.end())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl << std::endl;
    std::cout << "SERVER NAME: " << _server << std::endl << std::endl;
    std::cout << "CONTENT TYPE: " << _contentType << std::endl << std::endl;
    std::cout << "CONTENT LENGTH: " << _contentLength << std::endl << std::endl;
    std::cout << "CONTENT LANGUAGE: " << _contentLanguage << std::endl << std::endl;
    std::cout << "BODY: " << std::endl << _body << std::endl << std::endl;


    return (1);
}