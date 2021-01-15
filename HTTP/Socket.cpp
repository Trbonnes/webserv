/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:55 by trbonnes          #+#    #+#             */
/*   Updated: 2021/01/06 09:10:20 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() :
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

Socket::Socket(int fd) :
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

Socket::Socket(Socket const &copy)
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

Socket::~Socket() {
}

Socket      &Socket::operator=(Socket const &rhs)
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

int				Socket::getFd()
{
	return _fd;
}

std::string     Socket::getMethod()
{
    return _method;
}

std::string     Socket::getRequestURI()
{
    return _requestURI;
}

std::string     Socket::getHttpVersion()
{
    return _httpVersion;
}

std::string     Socket::getContentLength()
{
    return _contentLength;
}

std::string     Socket::getContentLocation()
{
    return _contentLocation;
}

std::string     Socket::getContentType()
{
    return _contentType;
}

bool    Socket::getMultipartContent()
{
    return _multipartContent;
}

std::string     Socket::getContentBoundary()
{
    return _contentBoundary;
}

std::string     Socket::getDate()
{
    return _date;
}

std::string     Socket::getTransferEncoding()
{
    return _transferEncoding;
}

std::vector<std::string>     &Socket::getAcceptCharset()
{
    return _acceptCharset;
}

std::vector<std::string>     &Socket::getAcceptLanguage()
{
    return _acceptLanguage;
}

std::string     Socket::getAuthorization()
{
    return _authorization;
}

std::string     Socket::getHost()
{
    return _host;
}

int				Socket::getPort() {
	return _port;
}

std::string     Socket::getUserAgent()
{
    return _userAgent;
}

std::string Socket::getReferer()
{
	return _referer;
}

std::string     Socket::getBody()
{
    return _body;
}

std::string     Socket::getRemoteAddr()
{
    return _addr;
}

std::string		Socket::getXSecret() {
	return _xSecret;
}


void	Socket::setFd(int fd) {
	_fd = fd;
}

void	Socket::setMethod(std::string method) {
	_method = method;
}

void	Socket::setRequestURI(std::string RequestURI) {
	_requestURI = RequestURI;
}

void	Socket::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

void	Socket::setContentLength(std::string ContentLength) {
	_contentLength = ContentLength;
}

void	Socket::setContentLocation(std::string ContentLocation) {
	_contentLocation = ContentLocation;
}

void	Socket::setContentType(std::string ContentType) {
	_contentType = ContentType;
}

void    Socket::setMultipartContent(bool multipart)
{
    _multipartContent = multipart;
}

void     Socket::setContentBoundary(std::string boundary)
{
    _contentBoundary = boundary;
}

void	Socket::setDate(std::string Date) {
	_date = Date;
}

void	Socket::setTransferEncoding(std::string TransferEncoding) {
	_transferEncoding = TransferEncoding;
}

void	Socket::setAcceptCharset(std::vector<std::string> AcceptCharset) {
	_acceptCharset = AcceptCharset;
}

void	Socket::setAcceptLanguage(std::vector<std::string> AcceptLanguage) {
	_acceptLanguage = AcceptLanguage;
}

void	Socket::setAuthorization(std::string Authorization) {
	_authorization = Authorization;
}

void	Socket::setHost(std::string Host) {
	_host = Host;
}

void	Socket::setPort(int port) {
	_port = port;
}

void	Socket::setUserAgent(std::string UserAgent) {
	_userAgent = UserAgent;
}

void	Socket::setReferer(std::string Referer) {
	_referer = Referer;
}

void	Socket::setBody(std::string &Body) {
	_body = Body;
}

void	Socket::setXSecret(std::string XSecret) {
	_xSecret = XSecret;
}

const char * Socket::ConnectionClose::what () const throw ()
{
	return "Connection has been closed";
}