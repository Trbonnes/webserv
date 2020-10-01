/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:30:52 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/01 15:46:15 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <fcntl.h>
#include "socket.hpp"

int main() {

	int fd = open("./httpRequestTest", O_RDONLY);
    Socket *socket = httpRequestParser(fd);

    std::cout << "Fd: " << socket->getFd() << std::endl;
    std::cout << "Method: " << socket->getMethod() << std::endl;
	std::cout << "URI: " << socket->getRequestURI() << std::endl;
    std::cout << "Version: " << socket->getHttpVersion() << std::endl;

    std::cout << "ContentType: " << socket->getContentType() << std::endl;
    std::cout << "getDate: " << socket->getDate() << std::endl;
    std::cout << "TransferEncoding: " << socket->getTransferEncoding() << std::endl;
    //std::cout << "AcceptCharset: " << socket->getAcceptCharset() << std::endl;
    //std::cout << "AcceptLanguage: " << socket->getAcceptLanguage() << std::endl;
    std::cout << "Authorization: " << socket->getAuthorization() << std::endl;
    std::cout << "Host: " << socket->getHost() << std::endl;
    std::cout << "UserAgent: " << socket->getUserAgent() << std::endl;
	std::cout << "Referer: " << socket->getReferer() << std::endl;
    std::cout << "ContentLocation: " << socket->getContentLocation() << std::endl;
    std::cout << "ContentLength: " << socket->getContentLength() << std::endl;
    //std::cout << "Body: " << socket->getBody() << std::endl;
}