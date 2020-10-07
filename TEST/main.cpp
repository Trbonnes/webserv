#include "Socket.hpp"
#include "../Methods.hpp"

int     main(void)
{
    Socket  socket;
    Methods method(socket);
    std::map<std::string, Config, CompNormalOrder<std::string> > Conf;

    Config server1;
    Config server2;

    method.getResponse();
    return 0;
}
