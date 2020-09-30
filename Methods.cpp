#include "Methods.hpp"

Methods::Methods() :
_socket(0),
_response(0),
_allow(""),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_contentLength(""),
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
_response(0),
_allow(""),
_wwwAuthenticate(""),
_referer(""),
_lastModified(""),
_location(""),
_server(""),
_contentLanguage(""),
_contentLength(""),
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
            callMethod(i);
        i++;
    }
    _statusCode = BAD_REQUEST;
}

Methods::Methods(Methods const &copy)
{
    _socket = copy._socket;
    _response = copy._response;
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
    return (1);
}