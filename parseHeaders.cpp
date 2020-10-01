/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeaders.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 18:55:18 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/01 15:18:41 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

void ParseAcceptCharsets(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseAcceptLanguage(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseAutorization(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseContentLength(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseContentLocation(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseContentType(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseDate(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseHost(Socket *socket, std::string request, size_t pos) {
	std::string s;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}
	socket->setHost(s);
}

void ParseReferer(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseTransferEncoding(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}

void ParseUserAgent(Socket *socket, std::string request, size_t pos) {
	(void)socket;
	(void)request;
	(void)pos;
}
