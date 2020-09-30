/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:55 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 18:12:19 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

Socket::Socket() :
_fd(0),
_method(),
_requestURI(),
_httpVersion(),
_contentLength(),
_contentLocation(),
_contentType(),
_date(),
_transferEncoding(),
_acceptCharset(),
_acceptLanguage(),
_authorization(),
_host(),
_userAgent(),
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
_date(),
_transferEncoding(),
_acceptCharset(),
_acceptLanguage(),
_authorization(),
_host(),
_userAgent(),
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
    _date = copy._date;
    _transferEncoding = copy._transferEncoding;
    _acceptCharset = copy._acceptCharset;
    _acceptLanguage = copy._acceptLanguage;
    _authorization = copy._authorization;
    _host = copy._host;
    _userAgent = copy._userAgent;
    _body = copy._body;
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
    _date = rhs._date;
    _transferEncoding = rhs._transferEncoding;
    _acceptCharset = rhs._acceptCharset;
    _acceptLanguage = rhs._acceptLanguage;
    _authorization = rhs._authorization;
    _host = rhs._host;
    _userAgent = rhs._userAgent;
    _body = rhs._body;
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

std::vector<std::string>     Socket::getContentLength()
{
    return _contentLength;
}

std::vector<std::string>     Socket::getContentLocation()
{
    return _contentLocation;
}

std::vector<std::string>     Socket::getContentType()
{
    return _contentType;
}

std::vector<std::string>     Socket::getDate()
{
    return _date;
}

std::vector<std::string>     Socket::getTransferEncoding()
{
    return _transferEncoding;
}

std::vector<std::string>     Socket::getAcceptCharset()
{
    return _acceptCharset;
}

std::vector<std::string>     Socket::getAcceptLanguage()
{
    return _acceptLanguage;
}

std::vector<std::string>     Socket::getAuthorization()
{
    return _authorization;
}

std::vector<std::string>     Socket::getHost()
{
    return _host;
}

std::vector<std::string>     Socket::getUserAgent()
{
    return _userAgent;
}

std::vector<std::string> Socket::getReferer()
{
	return _referer;
}

std::string     Socket::getBody()
{
    return _body;
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

void	Socket::setContentLength(std::vector<std::string> ContentLength) {
	_contentLength = ContentLength;
}

void	Socket::setContentLocation(std::vector<std::string> ContentLocation) {
	_contentLocation = ContentLocation;
}

void	Socket::setContentType(std::vector<std::string> ContentType) {
	_contentType = ContentType;
}

void	Socket::setDate(std::vector<std::string> Date) {
	_date = Date;
}

void	Socket::setTransferEncoding(std::vector<std::string> TransferEncoding) {
	_transferEncoding = TransferEncoding;
}

void	Socket::setAcceptCharset(std::vector<std::string> AcceptCharset) {
	_acceptCharset = AcceptCharset;
}

void	Socket::setAcceptLanguage(std::vector<std::string> AcceptLanguage) {
	_acceptLanguage = AcceptLanguage;
}

void	Socket::setAuthorization(std::vector<std::string> Authorization) {
	_authorization = Authorization;
}

void	Socket::setHost(std::vector<std::string> Host) {
	_host = Host;
}

void	Socket::setUserAgent(std::vector<std::string> UserAgent) {
	_userAgent = UserAgent;
}

void	Socket::setReferer(std::vector<std::string> Referer) {
	_referer = Referer;
}

void	Socket::setBody(std::string Body) {
	_body = Body;
}
