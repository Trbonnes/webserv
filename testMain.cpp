/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:30:52 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 15:51:59 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <fcntl.h>
#include "socket.hpp"

int main() {

	int fd = open("./httpRequestTest", O_RDONLY);
	Socket socket(fd);
	
    std::cout << socket.getFd() << std::endl;
    std::cout << socket.getMethod() << std::endl;
    std::cout << socket.getRequestURI() << std::endl;
    std::cout << socket.gethttpVersion() << std::endl;
    std::cout << socket.getContentLenght() << std::endl;
    std::cout << socket.getContentLocation() << std::endl;
    std::cout << socket.getContentType() << std::endl;
    std::cout << socket.getDate() << std::endl;
    std::cout << socket.getTransferEncoding() << std::endl;
    std::cout << socket.getAcceptCharset() << std::endl;
    std::cout << socket.getAcceptLanguage() << std::endl;
    std::cout << socket.getAuthorization() << std::endl;
    std::cout << socket.getHost() << std::endl;
    std::cout << socket.getUserAgent() << std::endl;
	std::cout << socket.getReferer() << std::endl;
    std::cout << socket.getBody() << std::endl;
}