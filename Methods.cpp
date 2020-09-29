#include "Methods.hpp"

Methods::Methods() :
_socket(0),
_response(0),
_methodsName({"GET", "HEAD", "POST", "PUT",
"DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"}),
_method({&Methods::get, &Methods::head,
&Methods::post, &Methods::put, &Methods::del,
&Methods::connect, &Methods::options, &Methods::trace,
&Methods::patch}) {}

Methods::Methods(Socket &socket) :
_socket(socket),
_response(0),
_methodsName({"GET", "HEAD", "POST", "PUT",
"DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"}),
_method({&Methods::get, &Methods::head,
&Methods::post, &Methods::put, &Methods::del,
&Methods::connect, &Methods::options, &Methods::trace,
&Methods::patch})
{
    int     i;

    i = 0;
    while (i < NB_METHODS)
    {
        if (_socket.getMethod().compare(_methodsName[i]))
            _method[i]();
        i++;
    }
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

void        Methods::get()
{
    
}