#include "HTTP/TEST/ConfigServer.hpp"
#include <fstream>
#include <fcntl.h>
#include "HTTP//HTTP.hpp"
#include "Socket/Socket.hpp"

int     main(void)
{
	int fd = open("/sgoinfre/goinfre/Perso/pganglof/webserv/Socket/httpRequestTest", O_RDWR);
    Socket *socket = httpRequestParser(fd);
    ConfigServer    config;
    HTTP method(socket, config);
    std::string response;

    response = method.getResponse();

    std::cout << std::endl << response << std::endl << std::endl;

//    write(fd, response.c_str(), response.length());
    
    return 0;
}


    // size_t i = 0;

    // std::cout << "Fd: " << socket->getFd() << std::endl;
    // std::cout << "Method: " << socket->getMethod() << std::endl;
	// std::cout << "URI: " << socket->getRequestURI() << std::endl;
    // std::cout << "Version: " << socket->gethttpVersion() << std::endl;

    // std::cout << "ContentType: " << std::endl;
    // while (i < socket->getContentType().size()){
    //     std::cout << socket->getContentType()[i] << std::endl;
    //     i++;
    // }
    // std::cout << "getDate: " << socket->getDate() << std::endl;
    // std::cout << "TransferEncoding: " << socket->getTransferEncoding() << std::endl;
    // i = 0;
    // std::cout << "AcceptCharset: " << std::endl;

    // std::vector<std::string>::iterator it1 = socket->getAcceptCharset().begin();

    // while (it1 != socket->getAcceptCharset().end())
    // {
    //     std::cout << *it1 << std::endl;
    //     it1++;
    // }

    // std::vector<std::string>::iterator it = socket->getAcceptLanguage().begin();

    // std::cout << "AcceptLanguage: " << std::endl;
    // while (it != socket->getAcceptLanguage().end())
    // {
    //     std::cout << *it << std::endl;
    //     it++;
    // }

    // // while (i < socket->getAcceptCharset().size()){
    // //     std::cout << socket->getAcceptCharset()[i] << std::endl;
    // //     i++;
    // // }
    // // i = 0;
    // // while (i < socket->getAcceptLanguage().size()){
    // //     std::cout << socket->getAcceptLanguage()[i] << std::endl;
    // //     i++;
    // // }

    // std::cout << "Authorization: " << socket->getAuthorization() << std::endl;
    // std::cout << "Host: " << socket->getHost() << std::endl;
    // std::cout << "UserAgent: " << socket->getUserAgent() << std::endl;
	// std::cout << "Referer: " << socket->getReferer() << std::endl;
    // std::cout << "ContentLocation: " << socket->getContentLocation() << std::endl;
    // std::cout << "ContentLength: " << socket->getContentLength() << std::endl;
    // i = 0;
    // std::cout << "Body: " << std::endl;
    // while (i < socket->getBody().size()){
    //     std::cout << socket->getBody()[i] << std::endl;
    //     i++;
    // }