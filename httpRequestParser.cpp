/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 18:53:37 by trbonnes         ###   ########.fr       */
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
	std::vector<std::string> v;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}
	v.push_back(s);
	socket->setHost(v);
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

int		httpRequestParseHeaders(std::string request, Socket *socket) {
	char const *x[] = {
		"Accept-Charsets",
		"Accept-Language",
		"Autorization",
		"Content-Length",
		"Content-Location",
		"Content-Type",
		"Date",
		"Host",
		"Referer",
		"Transfer-Encoding",
		"User-Agent"
	};
	void (*f[])(Socket*, std::string, size_t) = {
		&ParseAcceptCharsets,
		&ParseAcceptLanguage,
		&ParseAutorization,
		&ParseContentLength,
		&ParseContentLocation,
		&ParseContentType,
		&ParseDate,
		&ParseHost,
		&ParseReferer,
		&ParseTransferEncoding,
		&ParseUserAgent
	};
	std::vector<std::string> methodsVec(x, x + sizeof(x) / sizeof(*x));
	std::string	s2;
	size_t		pos;
	int			j = 0;

	for (std::vector<std::string>::iterator i = methodsVec.begin(); i != methodsVec.end(); i++) {
		pos = request.find(*i);
		if (pos != std::string::npos)
			f[j](socket, request, pos);
		j++;
	}

	return 0;
}

int		httpRequestParseRequestLine(std::string request, Socket *socket) {
	char const *x[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE"};
	std::vector<std::string> methodsVec(x, x + sizeof(x) / sizeof(*x));
	std::string s2;
	size_t pos;

	//GET METHOD FROM REQUEST
	for (std::vector<std::string>::iterator i = methodsVec.begin(); i != methodsVec.end(); i++) {
		pos = request.find(*i);
		if (pos != std::string::npos)
			break ;
	}
	if (pos == std::string::npos)
		return 400;
	while (request[pos] && request[pos] != ' ') {
		s2.push_back(request[pos++]);
	}
	socket->setMethod(s2);
	s2.clear();

	//GET REQUEST-URI
	while (request[pos] && request[pos] != '/') { pos++ ;}
	while (request[pos] && request[pos] != ' ') {
		s2.push_back(request[pos++]);
	}
	socket->setRequestURI(s2);
	s2.clear();

	//GET HTTP VERSION
	while (request[pos] && request[pos] != 'H') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s2.push_back(request[pos++]);
	}
	socket->setHttpVersion(s2);
	s2.clear();

	return 0;
}

Socket	*httpRequestParser(int fd) {

	Socket *socket = new Socket(fd);
	char c[4096];
	std::string request;

	for (int i = 0; i < 4096; i++)
		c[i] = '\0';
	while (int ret = read(fd, c, 4096) > 0) {
		if (ret == -1) { return NULL; }
		request.append(c);
		for (int i = 0; i < 4096; i++)
			c[i] = '\0';
	}
	
	httpRequestParseRequestLine(request, socket);
	httpRequestParseHeaders(request, socket);

	return socket;
}