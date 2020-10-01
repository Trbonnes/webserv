#include "Socket.hpp"
#include "../Methods.hpp"

int     main(void)
{
    Socket  socket;
    Methods method(socket);

    method.getResponse();
    return 0;
}
