/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:02 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 15:45:55 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>

class   Socket
{
    private:

    // SOCKET

    int             _fd;

    // REQUEST LINE

    std::string     _method;
    std::string     _requestURI;
    std::string     _httpVersion;

    // HEADERS

    std::string     _contentLength;
    std::string     _contentLocation;
    std::string     _contentType;
    std::string     _date;
    std::string     _transferEncoding;
    std::string     _acceptCharset;
    std::string     _acceptLanguage;
    std::string     _authorization;
    std::string     _host;
    std::string     _userAgent;
	std::string		_referer;

    // BODY

    std::string     _body;

    // PRIVATE FUNCTIONS

    void            parseHTTP(void);

    public:
    Socket();
    Socket(int fd);
    Socket(Socket const &copy);
    ~Socket();

    Socket          &operator=(Socket const &rhs);
    int             getFd();
    std::string     getMethod();
    std::string     getRequestURI();
    std::string     gethttpVersion();
    std::string     getContentLenght();
    std::string     getContentLocation();
    std::string     getContentType();
    std::string     getDate();
    std::string     getTransferEncoding();
    std::string     getAcceptCharset();
    std::string     getAcceptLanguage();
    std::string     getAuthorization();
    std::string     getHost();
    std::string     getUserAgent();
	std::string		getReferer();
    std::string     getBody();
};

#endif
