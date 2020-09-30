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

    std::list<std::string>	_contentLength;
    std::list<std::string>	_contentLocation;
    std::list<std::string>	_contentType;
    std::list<std::string>	_date;
    std::list<std::string>	_transferEncoding;
    std::list<std::string>	_acceptCharset;
    std::list<std::string>	_acceptLanguage;
    std::list<std::string>	_authorization;
    std::list<std::string>	_host;
    std::list<std::string>	_userAgent;
	std::list<std::string>	   _referer;

    // BODY

    std::string     _body;

    // PRIVATE FUNCTIONS

    public:
    Socket();
    Socket(int fd);
    Socket(Socket const &copy);
    ~Socket();

    Socket          &operator=(Socket const &rhs);
    int             		getFd();
    std::string     		getMethod();
    std::string     		getRequestURI();
    std::string     		gethttpVersion();
    std::list<std::string>	getContentLength();
    std::list<std::string>	getContentLocation();
    std::list<std::string>	getContentType();
    std::list<std::string>	getDate();
    std::list<std::string>	getTransferEncoding();
    std::list<std::string>	getAcceptCharset();
    std::list<std::string>	getAcceptLanguage();
    std::list<std::string>	getAuthorization();
    std::list<std::string>	getHost();
    std::list<std::string>	getUserAgent();
	std::list<std::string>	getReferer();
    std::string     		getBody();


    void	setFd(int fd);
    void	setMethod(std::string method);
    void	setRequestURI(std::string RequestURI);
    void	sethttpVersion(std::string httpVersion);
    void	setContentLength(std::list<std::string> ContentLength);
    void	setContentLocation(std::list<std::string> ContentLocation);
    void	setContentType(std::list<std::string> ContentType);
    void	setDate(std::list<std::string> Date);
    void	setTransferEncoding(std::list<std::string> TransferEncoding);
    void	setAcceptCharset(std::list<std::string> AcceptCharset);
    void	setAcceptLanguage(std::list<std::string> AcceptLanguage);
    void	setAuthorization(std::list<std::string> Authorization);
    void	setHost(std::list<std::string> Host);
    void	setUserAgent(std::list<std::string> UserAgent);
	void	setReferer(std::list<std::string> Referer);
    void	setBody(std::string Body);
};

Socket	*httpRequestParser(int fd);

#endif
