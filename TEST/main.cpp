#include "Socket.hpp"
#include "ConfigServer.hpp"
#include "../Methods.hpp"

int     main(void)
{
    Socket          socket;
    ConfigServer    config;

    Methods method(socket, config);

    method.getResponse();
    return 0;
}
