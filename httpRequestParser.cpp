/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/21 14:10:38 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int		httpRequestParseChunckedBody(std::string request, Socket *socket, size_t pos) {
	size_t chunkSize;
	std::string convert;
	std::string s = request.substr(pos, request.npos);
	std::vector<std::string>	bodyV;
	std::string body;

	try {
		pos = s.find("\r\n") - 1;
		convert = s.substr(pos, 1);
		chunkSize = std::stol(convert);
		while (chunkSize > 0) {
			bodyV.push_back(s.substr(pos + 3, chunkSize));
			s.erase(pos, 2 + chunkSize + 2);
			convert.clear();
			pos = s.find("\r\n") - 1;
			convert = s.substr(pos, 1);
			chunkSize = std::stol(convert);
		}
		for (size_t i = 0; i < bodyV.size(); i++) {
			body.append(bodyV[i]);
			body.append("\n");
		}
		bodyV.clear();
		socket->setBody(body);
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	
	return 0;
}

int		httpRequestParseBody(std::string request, Socket *socket) {
	std::string					content;
	std::string					body;
	size_t						chunkedPos = socket->getTransferEncoding().find("chunked");
	
	if (!socket->getContentLength().size() && chunkedPos == std::string::npos)
		return 0;
	
	size_t pos = request.find("\r\n\r\n");
	if (pos == request.npos) {
		pos = request.find("\n\n");
		pos += 2;
	}
	else 
		pos += 4;
	if (chunkedPos != std::string::npos)
		return httpRequestParseChunckedBody(request, socket, pos);

	try {
		body = request.substr(pos, request.npos);
		socket->setBody(body);
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}

	//FOR MULPART BODY : NOT USEFUL IN OUR IMPLEMENTATION
	// try {
	// 	if (!socket->getMultipartContent()) {
	// 		bodyV.push_back(request.substr(pos, request.npos));
	// 	}
	// 	else {
	// 		std::string s = request;
	// 		std::string boundary = "--" + socket->getContentBoundary();
	// 		std::string endBoundary = "--" + socket->getContentBoundary() + "--";
	// 		size_t endPos = s.find(endBoundary);
	// 		size_t boundPos;

	// 		content = socket->getContentType();
	// 		bodyV.push_back("");
	// 		s.erase(0, pos);
	// 		endPos = s.find(endBoundary);
	// 		while ((pos = s.find(boundary)) != endPos) {
	// 			s.erase(0, pos + boundary.length() + 1);
	// 			endPos = s.find(endBoundary);
	// 			boundPos = s.find(boundary);
	// 			pos = s.find("Content-Type");
	// 			if (pos >= boundPos) {
	// 				content.push_back("text/plain");
	// 				bodyV.push_back(s.substr(0, boundPos - 1));
	// 			}
	// 			else {
	// 				pos += 14;
	// 				content.push_back(s.substr(pos, s.find("\n", pos) - 14));
	// 				s.erase(0, s.find("\n", pos) + 1);;
	// 				boundPos = s.find(boundary);
	// 				bodyV.push_back(s.substr(0, boundPos - 1));
	// 			}
	// 			s.erase(0, boundPos);
	// 			endPos = s.find(endBoundary);
	// 		}
	// 		socket->setContentType(content);
	// 	}
	// }
	// catch (std::exception &e) {
	// 	std::cout << "Exception: " << e.what() << std::endl;
	// }
	//socket->setBody(bodyV);
	
	return 0;
}

int		httpRequestParseHeaders(std::string request, Socket *socket) {
	char const *x[] = {
		"Accept-Charset",
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
		&ParseAcceptCharset,
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
	std::vector<std::string> headerVec(x, x + sizeof(x) / sizeof(*x));
	std::string	s2;
	size_t		pos;
	int			j = 0;

	for (std::vector<std::string>::iterator i = headerVec.begin(); i != headerVec.end(); i++) {
		pos = request.find(*i);
		if (pos != request.npos)
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
		if (pos != request.npos)
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
	httpRequestParseBody(request, socket);

	return socket;
}
