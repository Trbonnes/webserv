#ifndef METHODS_HPP
# define METHODS_HPP

# include <iostream>
# include <array>
# include "Socket.hpp"

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

class   Methods
{
    private:

    Socket      _socket;
    int         _response;
    std::string _methodsName[NB_METHODS];
    void        (*_method[NB_METHODS])(void);

    Methods();

    static void        get(void);
    static void        head(void);
    static void        post(void);
    static void        put(void);
    static void        del(void);
    static void        connect(void);
    static void        options(void);
    static void        trace(void);
    static void        patch(void);

    
    public:
    Methods(Socket &socket);
    Methods(Methods const &copy);
    ~Methods();

    Methods     &operator=(Methods const &rhs);
    int         getResponse();
};

#endif