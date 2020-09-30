/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:30:52 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 18:52:37 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <fcntl.h>
#include "socket.hpp"

int main() {

	int fd = open("./httpRequestTest", O_RDONLY);
    Socket *socket = httpRequestParser(fd);

    std::cout << socket->getFd() << std::endl;
    std::cout << socket->getMethod() << std::endl;
	std::cout << socket->getRequestURI() << std::endl;
    std::cout << socket->getHttpVersion() << std::endl;
    /*
    std::cout << socket->getContentLength().front() << std::endl;
    std::cout << socket->getContentLocation().front() << std::endl;
    std::cout << socket->getContentType().front() << std::endl;
    std::cout << socket->getDate().front() << std::endl;
    std::cout << socket->getTransferEncoding().front() << std::endl;
    std::cout << socket->getAcceptCharset().front() << std::endl;
    std::cout << socket->getAcceptLanguage().front() << std::endl;
    std::cout << socket->getAuthorization().front() << std::endl;
    */
    std::cout << socket->getHost().front() << std::endl;
    /*
    std::cout << socket->getUserAgent().front() << std::endl;
	std::cout << socket->getReferer().front() << std::endl;
    std::cout << socket->getBody() << std::endl;
    */
}