// #include "Socket.hpp"
#include "ConfigServer.hpp"
#include <fstream>
#include <fcntl.h>
#include "../HTTP.hpp"
#include "../Socket/Socket.hpp"

Socket	*httpRequestParser(int fd);

int     main(void)
{

	int fd = open("../Socket/httpRequestTest", O_RDWR);
    Socket *socket = httpRequestParser(fd);



    // Socket          socket;
    ConfigServer    config;

    HTTP method(*socket, config);

    method.getResponse();
    return 0;
}
