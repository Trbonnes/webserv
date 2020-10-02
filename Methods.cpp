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
    _uri = socket.getRequestURI();
    setLocation();
    std::cout << "URI: " << _uri << std::endl;
    std::cout << "socket.getRequestURI(): " << socket.getRequestURI() << std::endl;
    std::cout << "Location: " << _location << std::endl;
    replaceURI(); 


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
    itBegin = getAllow(_uri).begin();
    itEnd = getAllow(_uri).end();
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

//** replace URI by the location **
void        Methods::replaceURI()
{
    _uri.replace(_uri.find(_location), _location.length(), getRoot(_location));
}

void        Methods::setLocation()
{
    _location = getLocation(_uri);
}

int         Methods::getResponse()
{
    std::vector<std::string>::iterator it;
    std::cout << std::endl;

    std::cout << "ORIGINALE URI: " << _socket.getRequestURI() << std::endl << std::endl;
    std::cout << "LOCATION: " << _location << std::endl << std::endl;
    std::cout << "URI: " << _uri << std::endl << std::endl;
    std::cout << "ROUTE: " << _route << std::endl << std::endl;
    std::cout << "SERVER NAME: " << _server << std::endl << std::endl;
    std::cout << "STATUS CODE: " << _statusCode << std::endl << std::endl;

    it = _allow.begin();
    std::cout << "ALLOW: ";
    while (it != _allow.end())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl << std::endl;
    std::cout << "CONTENT TYPE: " << _contentType << std::endl << std::endl;
    std::cout << "CONTENT LENGTH: " << _contentLength << std::endl << std::endl;
    std::cout << "CONTENT LANGUAGE: " << _contentLanguage << std::endl << std::endl;
    std::cout << "BODY: " << std::endl << _body << std::endl << std::endl;


    return (1);
}