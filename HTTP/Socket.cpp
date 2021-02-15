/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:55 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/15 15:24:06 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

httpRequest::httpRequest() :
_fd(0),
_method(),
_requestURI(),
_httpVersion(),
_contentLength(),
_contentLocation(),
_contentType(),
_multipartContent(),
_contentBoundary(),
_date(),
_transferEncoding(),
_acceptCharset(),
_acceptLanguage(),
_authorization(),
_host(),
_port(),
_userAgent(),
_xSecret(),
_body() {
}

httpRequest::httpRequest(int fd) :
_fd(fd),
_method(),
_requestURI(),
_httpVersion(),
_contentLength(),
_contentLocation(),
_contentType(),
_multipartContent(),
_contentBoundary(),
_date(),
_transferEncoding(),
_acceptCharset(),
_acceptLanguage(),
_authorization(),
_host(),
_port(),
_userAgent(),
_xSecret(),
_body() {
}

httpRequest::httpRequest(httpRequest const &copy)
{
    _fd = copy._fd;
    _method = copy._method;
    _requestURI = copy._requestURI;
    _httpVersion = copy._httpVersion;
    _contentLength = copy._contentLength;
    _contentLocation = copy._contentLocation;
    _contentType = copy._contentType;
    _multipartContent = copy._multipartContent;
    _contentBoundary = copy._contentBoundary;
    _date = copy._date;
    _transferEncoding = copy._transferEncoding;
    _acceptCharset = copy._acceptCharset;
    _acceptLanguage = copy._acceptLanguage;
    _authorization = copy._authorization;
    _host = copy._host;
	_port = copy._port;
    _userAgent = copy._userAgent;
    _body = copy._body;
	_xSecret = copy._xSecret;
}

httpRequest::~httpRequest() {
}

httpRequest      &httpRequest::operator=(httpRequest const &rhs)
{
    _fd = rhs._fd;
    _method = rhs._method;
    _requestURI = rhs._requestURI;
    _httpVersion = rhs._httpVersion;
    _contentLength = rhs._contentLength;
    _contentLocation = rhs._contentLocation;
    _contentType = rhs._contentType;
    _multipartContent = rhs._multipartContent;
    _contentBoundary = rhs._contentBoundary;
    _date = rhs._date;
    _transferEncoding = rhs._transferEncoding;
    _acceptCharset = rhs._acceptCharset;
    _acceptLanguage = rhs._acceptLanguage;
    _authorization = rhs._authorization;
    _host = rhs._host;
	_port = rhs._port;
    _userAgent = rhs._userAgent;
    _body = rhs._body;
	_xSecret = rhs._xSecret;
    return *this;
}

int				httpRequest::getFd()
{
	return _fd;
}

std::string     httpRequest::getMethod()
{
    return _method;
}

std::string     httpRequest::getRequestURI()
{
    return _requestURI;
}

std::string     httpRequest::getHttpVersion()
{
    return _httpVersion;
}

std::string     httpRequest::getContentLength()
{
    return _contentLength;
}

std::string     httpRequest::getContentLocation()
{
    return _contentLocation;
}

std::string     httpRequest::getContentType()
{
    return _contentType;
}

bool    httpRequest::getMultipartContent()
{
    return _multipartContent;
}

std::string     httpRequest::getContentBoundary()
{
    return _contentBoundary;
}

std::string     httpRequest::getDate()
{
    return _date;
}

std::string     httpRequest::getTransferEncoding()
{
    return _transferEncoding;
}

std::vector<std::string>     &httpRequest::getAcceptCharset()
{
    return _acceptCharset;
}

std::vector<std::string>     &httpRequest::getAcceptLanguage()
{
    return _acceptLanguage;
}

std::string     httpRequest::getAuthorization()
{
    return _authorization;
}

std::string     httpRequest::getHost()
{
    return _host;
}

int				httpRequest::getPort() {
	return _port;
}

std::string     httpRequest::getUserAgent()
{
    return _userAgent;
}

std::string httpRequest::getReferer()
{
	return _referer;
}

std::string     &httpRequest::getBody()
{
    return _body;
}

std::string     httpRequest::getRemoteAddr()
{
    return _addr;
}

std::string		httpRequest::getXSecret() {
	return _xSecret;
}


void	httpRequest::setFd(int fd) {
	_fd = fd;
}

void	httpRequest::setMethod(std::string method) {
	_method = method;
}

void	httpRequest::setRequestURI(std::string RequestURI) {
	_requestURI = RequestURI;
}

void	httpRequest::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

void	httpRequest::setContentLength(std::string ContentLength) {
	_contentLength = ContentLength;
}

void	httpRequest::setContentLocation(std::string ContentLocation) {
	_contentLocation = ContentLocation;
}

void	httpRequest::setContentType(std::string ContentType) {
	_contentType = ContentType;
}

void    httpRequest::setMultipartContent(bool multipart)
{
    _multipartContent = multipart;
}

void     httpRequest::setContentBoundary(std::string boundary)
{
    _contentBoundary = boundary;
}

void	httpRequest::setDate(std::string Date) {
	_date = Date;
}

void	httpRequest::setTransferEncoding(std::string TransferEncoding) {
	_transferEncoding = TransferEncoding;
}

void	httpRequest::setAcceptCharset(std::vector<std::string> AcceptCharset) {
	_acceptCharset = AcceptCharset;
}

void	httpRequest::setAcceptLanguage(std::vector<std::string> AcceptLanguage) {
	_acceptLanguage = AcceptLanguage;
}

void	httpRequest::setAuthorization(std::string Authorization) {
	_authorization = Authorization;
}

void	httpRequest::setHost(std::string Host) {
	_host = Host;
}

void	httpRequest::setPort(int port) {
	_port = port;
}

void	httpRequest::setUserAgent(std::string UserAgent) {
	_userAgent = UserAgent;
}

void	httpRequest::setReferer(std::string Referer) {
	_referer = Referer;
}

void	httpRequest::setBody(std::string &Body) {
	_body = Body;
}

void	httpRequest::setXSecret(std::string XSecret) {
	_xSecret = XSecret;
}

const char * httpRequest::ConnectionClose::what () const throw ()
{
	return "Connection has been closed";
}