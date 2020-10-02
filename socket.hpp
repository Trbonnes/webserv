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

    std::string	                _contentLength;
    std::string	                _contentLocation;
    std::string	                _contentType;
    std::string	                _date;
    std::string	                _transferEncoding;
    std::vector<std::string>	_acceptCharset;
    std::vector<std::string>	_acceptLanguage;
    std::string                 _authorization;
    std::string	                _host;
    std::string	                _userAgent;
	std::string	                _referer;

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
    std::string	                getContentLength();
    std::string	                getContentLocation();
    std::string	                getContentType();
    std::string	                getDate();
    std::string	                getTransferEncoding();
    std::vector<std::string>	getAcceptCharset();
    std::vector<std::string>	getAcceptLanguage();
    std::string	                getAuthorization();
    std::string	                getHost();
    std::string	                getUserAgent();
	std::string	                getReferer();
    std::string     			getBody();


    void	setFd(int fd);
    void	setMethod(std::string method);
    void	setRequestURI(std::string RequestURI);
    void	setHttpVersion(std::string httpVersion);
    void	setContentLength(std::string ContentLength);
    void	setContentLocation(std::string ContentLocation);
    void	setContentType(std::string ContentType);
    void	setDate(std::string Date);
    void	setTransferEncoding(std::string TransferEncoding);
    void	setAcceptCharset(std::vector<std::string> AcceptCharset);
    void	setAcceptLanguage(std::vector<std::string> AcceptLanguage);
    void	setAuthorization(std::string Authorization);
    void	setHost(std::string Host);
    void	setUserAgent(std::string UserAgent);
	void	setReferer(std::string Referer);
    void	setBody(std::string Body);
};

Socket	*httpRequestParser(int fd);

void	ParseAcceptCharset(Socket *socket, std::string request, size_t pos);
void	ParseAcceptLanguage(Socket *socket, std::string request, size_t pos);
void	ParseAutorization(Socket *socket, std::string request, size_t pos);
void	ParseContentLength(Socket *socket, std::string request, size_t pos);
void	ParseContentLocation(Socket *socket, std::string request, size_t pos);
void	ParseContentType(Socket *socket, std::string request, size_t pos);
void	ParseDate(Socket *socket, std::string request, size_t pos);
void	ParseHost(Socket *socket, std::string request, size_t pos);
void	ParseReferer(Socket *socket, std::string request, size_t pos);
void	ParseTransferEncoding(Socket *socket, std::string request, size_t pos);
void	ParseUserAgent(Socket *socket, std::string request, size_t pos);

#endif
