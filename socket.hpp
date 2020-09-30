/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>	     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:02 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 17:08:46 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
# include <list>
# include <vector>
# include <unistd.h>

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

    std::vector<std::string>	_contentLength;
    std::vector<std::string>	_contentLocation;
    std::vector<std::string>	_contentType;
    std::vector<std::string>	_date;
    std::vector<std::string>	_transferEncoding;
    std::vector<std::string>	_acceptCharset;
    std::vector<std::string>	_acceptLanguage;
    std::vector<std::string>	_authorization;
    std::vector<std::string>	_host;
    std::vector<std::string>	_userAgent;
	std::vector<std::string>	_referer;

    // BODY

    std::string     _body;

    // PRIVATE FUNCTIONS

    public:
    Socket();
    Socket(int fd);
    Socket(Socket const &copy);
    ~Socket();

    Socket          &operator=(Socket const &rhs);
    int             			getFd();
    std::string     			getMethod();
    std::string     			getRequestURI();
    std::string     			getHttpVersion();
    std::vector<std::string>	getContentLength();
    std::vector<std::string>	getContentLocation();
    std::vector<std::string>	getContentType();
    std::vector<std::string>	getDate();
    std::vector<std::string>	getTransferEncoding();
    std::vector<std::string>	getAcceptCharset();
    std::vector<std::string>	getAcceptLanguage();
    std::vector<std::string>	getAuthorization();
    std::vector<std::string>	getHost();
    std::vector<std::string>	getUserAgent();
	std::vector<std::string>	getReferer();
    std::string     			getBody();


    void	setFd(int fd);
    void	setMethod(std::string method);
    void	setRequestURI(std::string RequestURI);
    void	setHttpVersion(std::string httpVersion);
    void	setContentLength(std::vector<std::string> ContentLength);
    void	setContentLocation(std::vector<std::string> ContentLocation);
    void	setContentType(std::vector<std::string> ContentType);
    void	setDate(std::vector<std::string> Date);
    void	setTransferEncoding(std::vector<std::string> TransferEncoding);
    void	setAcceptCharset(std::vector<std::string> AcceptCharset);
    void	setAcceptLanguage(std::vector<std::string> AcceptLanguage);
    void	setAuthorization(std::vector<std::string> Authorization);
    void	setHost(std::vector<std::string> Host);
    void	setUserAgent(std::vector<std::string> UserAgent);
	void	setReferer(std::vector<std::string> Referer);
    void	setBody(std::string Body);
};

Socket	*httpRequestParser(int fd);

#endif
