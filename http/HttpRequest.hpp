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

#ifndef HTTP_REQUEST
# define HTTP_REQUEST


# define READ 131072

# include <iostream>
# include <string>
# include <list>
# include <vector>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <errno.h>

# include "core/utils/utils.hpp"
# include "statusCodes.hpp"
# include "core/Log.hpp"
# include "LoadController.hpp"

class   HttpRequest
{
    private:

    // SOCKET
    std::string                 _addr;

    // REQUEST LINE

    std::string                 _method;
    std::string                 _requestURI;
    std::string                 _httpVersion;

    // HEADERS

    std::string	                _contentLength;
    std::string	                _contentLocation;
    std::string   				_contentType;
    bool                        _multipartContent;
    std::string                 _contentBoundary;
    std::string	                _date;
    std::string	                _transferEncoding;
    std::vector<std::string>	_acceptCharset;
    std::vector<std::string>	_acceptLanguage;
    std::string                 _authorization;
    std::string	                _host;
	int							_port;
    std::string	                _userAgent;
	std::string	                _referer;
	std::string	                _xSecret;

    // BODY

    std::string					_body;

    // PRIVATE FUNCTIONS

    public:
    HttpRequest();
    HttpRequest(HttpRequest const &copy);
    ~HttpRequest();

    HttpRequest                 &operator=(HttpRequest const &rhs);
    std::string&     			getMethod();
    std::string&     			getRequestURI();
    std::string&     			getHttpVersion();
    std::string&	            getContentLength();
    std::string&	            getContentLocation();
    std::string&				getContentType();
    bool                        getMultipartContent();
    std::string&                getContentBoundary();
    std::string&                getDate();
    std::string&                getTransferEncoding();
    std::vector<std::string>	&getAcceptCharset();
    std::vector<std::string>	&getAcceptLanguage();
    std::string&                getAuthorization();
    std::string&                getHost();
	int							getPort();
    std::string&                getUserAgent();
	std::string&                getReferer();
    std::string&				getBody();
    std::string&                getRemoteAddr();
    std::string&                getXSecret();


    void	setMethod(std::string method);
    void	setRequestURI(std::string RequestURI);
    void	setHttpVersion(std::string httpVersion);
    void	setContentLength(std::string ContentLength);
    void	setContentLocation(std::string ContentLocation);
    void	setContentType(std::string ContentType);
    void    setMultipartContent(bool multipart);
    void    setContentBoundary(std::string boundary);
    void	setDate(std::string Date);
    void	setTransferEncoding(std::string TransferEncoding);
    void	setAcceptCharset(std::vector<std::string> AcceptCharset);
    void	setAcceptLanguage(std::vector<std::string> AcceptLanguage);
    void	setAuthorization(std::string Authorization);
    void	setHost(std::string Host);
	void	setPort(int port);
    void	setUserAgent(std::string UserAgent);
	void	setReferer(std::string Referer);
    void	setBody(std::string &sBody);
	void	setXSecret(std::string XSecret);

    static HttpRequest* parseRequest(std::string&);

    class   ConnectionClose : public std::exception
	{
		public:
			const char * what () const throw ();
	};    
};


void	ParseAcceptCharset(HttpRequest *socket, std::string& request, size_t pos);
void	ParseAcceptLanguage(HttpRequest *socket, std::string& request, size_t pos);
void	ParseAutorization(HttpRequest *socket, std::string& request, size_t pos);
void	ParseContentLength(HttpRequest *socket, std::string& request, size_t pos);
void	ParseContentLocation(HttpRequest *socket, std::string& request, size_t pos);
void	ParseContentType(HttpRequest *socket, std::string& request, size_t pos);
void	ParseDate(HttpRequest *socket, std::string& request, size_t pos);
void	ParseHost(HttpRequest *socket, std::string& request, size_t pos);
void	ParseReferer(HttpRequest *socket, std::string& request, size_t pos);
void	ParseTransferEncoding(HttpRequest *socket, std::string& request, size_t pos);
void	ParseUserAgent(HttpRequest *socket, std::string& request, size_t pos);
void	ParseXSecret(HttpRequest *socket, std::string& request, size_t pos);
std::string ParseStdHeaders(std::string& request, size_t pos);

#endif