/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2020/12/11 11:11:26 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

int		httpRequestParseChunckedBody(std::string request, Socket *socket, size_t pos) {
	size_t chunkSize;
	std::string convert;
	std::string s = request.substr(pos, request.npos);
	std::vector<std::string>	bodyV;
	std::string body;

	Log::debug("\033[0;32mCHUNCKED");
	// Log::debug(s);
	pos = s.find("0\r\n");
	if (pos == s.npos) {
		char	c[8192];
		int		ret;

		Log::debug("\033[0;32mRead in chuncked body");
		while ((pos = s.find("0\r\n")) == s.npos) {
			ft_bzero(c, 8192);
			ret = read(socket->getFd(), c, 8192);
			Log::debug("\033[0;32mread in chuncked ret: ");
			Log::debug(ret);
			s.append(c, ret);
			// Log::debug("s:");
			// Log::debug(s);
		}
	}
	try {
		chunkSize = 0;
		pos = 0;
		// Log::debug("s:");
		// Log::debug(s);
		// pos = s.find("\r\n") - 1;
		// while (pos > 0) {
		// 	pos--;
		// }
		while (s[pos] && s[pos] != '\r') {
			convert.append(s.substr(pos, 1));
			pos++;
		}
		chunkSize = strtol(convert.c_str(), NULL, 16);
		Log::debug("chunck Size: ");
		Log::debug(chunkSize);
		socket->setContentLength(ft_itoa(atol(socket->getContentLength().c_str()) + chunkSize));
		while (chunkSize > 0) {
			Log::debug("new chunk");
			bodyV.push_back(s.substr(convert.length() + 3, chunkSize));
			s.erase(0, convert.length() + 2 + chunkSize + 2);
			convert.clear();
			pos = 0;
			// pos = s.find("\r\n") - 1;
			// Log::debug("pos found");
			// while (pos > 0) {
			// 	pos--;
			// }
			// Log::debug("lol");
			while (s[pos] && s[pos] != '\r') {
				convert.append(s.substr(pos, 1));
				pos++;
			}
			Log::debug("lolilol");
			chunkSize = strtol(convert.c_str(), NULL, 16);
			Log::debug("chunck Size: ");
			Log::debug(chunkSize);
			socket->setContentLength(ft_itoa(atol(socket->getContentLength().c_str()) + chunkSize));
		}
		Log::debug("All chunk read");
		for (size_t i = 0; i < bodyV.size(); i++) {
			body.append(bodyV[i]);
			body.append("\n");
		}
		bodyV.clear();
		socket->setBody(body);
		Log::debug("\033[0;32mchuncked content length: ");
		Log::debug(socket->getContentLength().c_str());
	}
	catch (std::exception &e) {
		std::cerr << "Exception1: " << e.what() << std::endl;
	}
	
	return 0;
}

int		httpRequestParseBody(std::string request, Socket *socket) {
	std::string					content;
	std::string					body;
	size_t						chunkedPos = socket->getTransferEncoding().find("chunked");
	size_t						ret;
	
	if (!socket->getContentLength().size() && chunkedPos == std::string::npos){
		socket->setBody("");
		return 0;
	}
	
	size_t pos = request.find("\r\n\r\n");
	if (pos == request.npos) {
		pos = request.find("\n\n");
		pos += 2;
	}
	else 
		pos += 4;
	if (pos >= request.npos) {
		socket->setBody("");
		return 0;
	}
	if (chunkedPos != std::string::npos)
		return httpRequestParseChunckedBody(request, socket, pos);

	try {
		body = request.substr(pos, request.npos);
		size_t	contentLength = atol(socket->getContentLength().c_str());
		std::cout << body.size() << std::endl;
		if (body.size() >= contentLength)
			socket->setBody(body);
		else {
			char	c[8192];
			
			Log::debug("\033[0;32mRead if body missing");
			while (body.size() <= contentLength) {
				ft_bzero(c, 8192);
				ret = read(socket->getFd(), c, 8192);
				body.append(c, ret);
			}
			if (body.size() > contentLength)
				body.erase(contentLength, body.npos);
			socket->setBody(body);
			// Log::debug(body);
		}
	}
	catch (std::exception &e) {
		std::cerr << "Exception2: " << e.what() << std::endl;
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
	while (request[pos] && request[pos] == ' ') { pos++ ;}
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

	return 0;
}

Socket	*httpRequestParser(int fd) {

	Socket *socket;
	char	c[8192];
	int		ret;
	std::string request;

	Log::debug("\033[0;32mRequestParsing Reading");
	// ft_bzero(c, 8192);
	// ret = read(fd, c, 8192);
	// Log::debug("\033[0;32mret: ");
	// Log::debug(ret);
	// if (ret == -1)
	// 	throw Socket::BadReadException();
	// request.append(c, ret);
	// ft_bzero(c, 8192);
	// Log::debug("\"");
	// Log::debug(request.c_str());
	// Log::debug("\"");
	// Log::debug("\033[0;32mret: ");
	// Log::debug(ret);
	// if (request.find("chunked") !=  request.npos) {
	// 	Log::debug("chuncked");
	// 	if (request.find("0\r\n") == request.npos)
	// 		return NULL;
	// }
	// else if (request.find("Content-Length") != request.npos) {
	// 	Log::debug("with body");
	// 	if (request.find("\r\n\r\n") == request.npos)
	// 		return NULL;
	// 	else if (request.find("HEAD") == request.npos){
	// 		Log::debug("not HEAD");
	// 		size_t pos = request.find("\r\n\r\n");
	// 		size_t pos2 = request.find("Content-Length");

	// 		int length = atoi(ParseStdHeaders(request, pos2).c_str());
	// 		if ((request.length() - pos + 3) < (size_t)length)
	// 			return NULL;
	// 	}
	// }
	// else if (request.find("\r\n\r\n") == request.npos && ret != 0) {
	// 	return NULL;
	// }

	int i = 0;
	while (request.find("\r\n\r\n") >= request.npos && request.find("\n\n") >= request.npos)
	{
		ft_bzero(c, 8192);
		ret = read(fd, c, 8192);
		i += ret;
		Log::debug("\033[0;32mret: ");
		std::cerr << "i: " << i << std::endl;
		// Log::debug(ret);
		if (ret == 0)
			throw HttpConnection::ConnectionClose();
		// Log::debug("\033[0;32mret: ");
		// Log::debug(ret);
		if (ret == -1)
			throw Socket::BadReadException();
		request.append(c, ret);
	}

	Log::debug("\033[0;32mRequestParsing Reading End");
	// Log::debug(request.c_str());
	socket = new Socket(fd);
	Log::debug("\033[0;32mRequestParsing Creation");
	httpRequestParseRequestLine(request, socket);
		// std::cerr << "BLABLA2" << std::endl;
	httpRequestParseHeaders(request, socket);
		// std::cerr << "BLABLA3" << std::endl;
	httpRequestParseBody(request, socket);
	request.clear();

	std::cerr << "HTTP REQUEST PARSER" << std::endl;
	return socket;
}