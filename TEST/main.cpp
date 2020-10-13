#include "Socket.hpp"
#include "ConfigServer.hpp"
#include "../HTTP.hpp"

int     main(void)
{
    Socket          socket;
    ConfigServer    config;

    HTTP method(socket, config);

    method.getResponse();
    return 0;
}
