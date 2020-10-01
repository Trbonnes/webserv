/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeaders.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 18:55:18 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/01 15:43:09 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"


std::string ParseStdHeaders(std::string request, size_t pos) {
	std::string s;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}
	return s;
}

std::vector<std::string> ParseAcceptHeaders(std::string request, size_t pos) {
	std::vector<std::string> v;
	(void)request;
	(void)pos;
	return v;
}

void ParseAcceptCharsets(Socket *socket, std::string request, size_t pos) {
	socket->setAcceptCharset(ParseAcceptHeaders(request, pos));
}

void ParseAcceptLanguage(Socket *socket, std::string request, size_t pos) {
	socket->setAcceptLanguage(ParseAcceptHeaders(request, pos));
}

void ParseAutorization(Socket *socket, std::string request, size_t pos) {
	socket->setAuthorization(ParseStdHeaders(request, pos));
}

void ParseContentLength(Socket *socket, std::string request, size_t pos) {
	socket->setContentLength(ParseStdHeaders(request, pos));
}

void ParseContentLocation(Socket *socket, std::string request, size_t pos) {
	socket->setContentLocation(ParseStdHeaders(request, pos));
}

void ParseContentType(Socket *socket, std::string request, size_t pos) {
	socket->setContentType(ParseStdHeaders(request, pos));
}

void ParseDate(Socket *socket, std::string request, size_t pos) {
	socket->setDate(ParseStdHeaders(request, pos));
}

void ParseHost(Socket *socket, std::string request, size_t pos) {
	socket->setHost(ParseStdHeaders(request, pos));
}

void ParseReferer(Socket *socket, std::string request, size_t pos) {
	socket->setReferer(ParseStdHeaders(request, pos));
}

void ParseTransferEncoding(Socket *socket, std::string request, size_t pos) {
	socket->setTransferEncoding(ParseStdHeaders(request, pos));
}

void ParseUserAgent(Socket *socket, std::string request, size_t pos) {
	socket->setUserAgent(ParseStdHeaders(request, pos));
}
