/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:10:55 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/15 16:35:38 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() :
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

HttpRequest::HttpRequest(int fd) :
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

HttpRequest::HttpRequest(HttpRequest const &copy)
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

HttpRequest::~HttpRequest() {
}

HttpRequest      &HttpRequest::operator=(HttpRequest const &rhs)
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

int				HttpRequest::getFd()
{
	return _fd;
}

std::string     HttpRequest::getMethod()
{
    return _method;
}

std::string     HttpRequest::getRequestURI()
{
    return _requestURI;
}

std::string     HttpRequest::getHttpVersion()
{
    return _httpVersion;
}

std::string     HttpRequest::getContentLength()
{
    return _contentLength;
}

std::string     HttpRequest::getContentLocation()
{
    return _contentLocation;
}

std::string     HttpRequest::getContentType()
{
    return _contentType;
}

bool    HttpRequest::getMultipartContent()
{
    return _multipartContent;
}

std::string     HttpRequest::getContentBoundary()
{
    return _contentBoundary;
}

std::string     HttpRequest::getDate()
{
    return _date;
}

std::string     HttpRequest::getTransferEncoding()
{
    return _transferEncoding;
}

std::vector<std::string>     &HttpRequest::getAcceptCharset()
{
    return _acceptCharset;
}

std::vector<std::string>     &HttpRequest::getAcceptLanguage()
{
    return _acceptLanguage;
}

std::string     HttpRequest::getAuthorization()
{
    return _authorization;
}

std::string     HttpRequest::getHost()
{
    return _host;
}

int				HttpRequest::getPort() {
	return _port;
}

std::string     HttpRequest::getUserAgent()
{
    return _userAgent;
}

std::string HttpRequest::getReferer()
{
	return _referer;
}

std::string     &HttpRequest::getBody()
{
    return _body;
}

std::string     HttpRequest::getRemoteAddr()
{
    return _addr;
}

std::string		HttpRequest::getXSecret() {
	return _xSecret;
}


void	HttpRequest::setFd(int fd) {
	_fd = fd;
}

void	HttpRequest::setMethod(std::string method) {
	_method = method;
}

void	HttpRequest::setRequestURI(std::string RequestURI) {
	_requestURI = RequestURI;
}

void	HttpRequest::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

void	HttpRequest::setContentLength(std::string ContentLength) {
	_contentLength = ContentLength;
}

void	HttpRequest::setContentLocation(std::string ContentLocation) {
	_contentLocation = ContentLocation;
}

void	HttpRequest::setContentType(std::string ContentType) {
	_contentType = ContentType;
}

void    HttpRequest::setMultipartContent(bool multipart)
{
    _multipartContent = multipart;
}

void     HttpRequest::setContentBoundary(std::string boundary)
{
    _contentBoundary = boundary;
}

void	HttpRequest::setDate(std::string Date) {
	_date = Date;
}

void	HttpRequest::setTransferEncoding(std::string TransferEncoding) {
	_transferEncoding = TransferEncoding;
}

void	HttpRequest::setAcceptCharset(std::vector<std::string> AcceptCharset) {
	_acceptCharset = AcceptCharset;
}

void	HttpRequest::setAcceptLanguage(std::vector<std::string> AcceptLanguage) {
	_acceptLanguage = AcceptLanguage;
}

void	HttpRequest::setAuthorization(std::string Authorization) {
	_authorization = Authorization;
}

void	HttpRequest::setHost(std::string Host) {
	_host = Host;
}

void	HttpRequest::setPort(int port) {
	_port = port;
}

void	HttpRequest::setUserAgent(std::string UserAgent) {
	_userAgent = UserAgent;
}

void	HttpRequest::setReferer(std::string Referer) {
	_referer = Referer;
}

void	HttpRequest::setBody(std::string &Body) {
	_body = Body;
}

void	HttpRequest::setXSecret(std::string XSecret) {
	_xSecret = XSecret;
}

const char * HttpRequest::ConnectionClose::what () const throw ()
{
	return "Connection has been closed";
}