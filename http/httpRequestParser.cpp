/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/22 01:37:33 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

// static int		httpRequestParseChunkedBody(std::string request, HttpRequest *socket, size_t pos) {
// 	size_t chunkSize;
// 	std::string convert;
// 	std::string s = request.substr(pos, request.npos);
// 	std::string body;
// 	int	fd_read = socket->getFd();
// 	// Log::debug("\033[0;32mCHUNCKED");

// 	LoadController::loadController(10000000, body);
// 	try {
// 		char	c[READ];
// 		int		ret;
		
// 		while ((pos = s.find("\r\n")) == s.npos) {
// 			ft_bzero(c, READ);
// 			ret = read(fd_read, c, 1);
// 			s.append(c, ret);
// 		}
// 		pos = 0;
// 		while (s[pos] && s[pos] != '\r') {
// 			convert.append(s.substr(pos, 1));
// 			pos++;
// 		}
// 		chunkSize = strtol(convert.c_str(), NULL, 16);
// 		// Log::debug("chunck Size: ");
// 		// Log::debug(chunkSize);
// 		while (chunkSize > 0) {
// 			// Log::debug("new chunk");
// 			char *tmp = ft_itoa(atol(socket->getContentLength().c_str()) + chunkSize);
// 			socket->setContentLength(tmp);
// 			free(tmp);
// 			s.clear();
// 			convert.clear();
// 			for (int i = 0; i < (int)chunkSize; i += ret) {
// 				if (chunkSize - i > READ) {
// 					ft_bzero(c, READ);
// 					ret = read(fd_read, c, READ);
// 					body.append(c, ret);
// 				}
// 				else {
// 					ft_bzero(c, READ);
// 					ret = read(fd_read, c, chunkSize - i);
// 					body.append(c, ret);
// 				}
// 			}
// 			ft_bzero(c, 8192);
// 			ret = read(fd_read, c, 2);
// 			while ((pos = s.find("\r\n")) == s.npos) {
// 				ft_bzero(c, 2);
// 				ret = read(fd_read, c, 1);
// 				s.append(c, ret);
// 			}
// 			pos = 0;
// 			while (s[pos] && s[pos] != '\r') {
// 				convert.append(s.substr(pos, 1));
// 				pos++;
// 			}
// 			chunkSize = strtol(convert.c_str(), NULL, 16);
// 			// Log::debug("chunck Size: ");
// 			// Log::debug(chunkSize);
// 		}
// 		// Log::debug("All chunk read");
// 		socket->setBody(body);
// 		// Log::debug("\033[0;32mchuncked content length: ");
// 		// Log::debug(socket->getContentLength().c_str());
// 		// body.clear();
// 	}
// 	catch (std::exception &e) {
// 		std::cerr << "Exception1: " << e.what() << std::endl;
// 	}
// 	return 0;
// }

// static int		httpRequestParseBody(std::string request, HttpRequest *socket) {
// 	std::string					content;
// 	std::string					body = "";
// 	size_t						chunkedPos = socket->getTransferEncoding().find("chunked");
// 	size_t						ret;

// 	// Log::debug(request.c_str());

// 	if (!socket->getContentLength().size() && chunkedPos == std::string::npos){
// 		socket->setBody(body);
// 		return 0;
// 	}
	
// 	size_t pos = request.find("\r\n\r\n");
// 	if (pos == request.npos) {
// 		pos = request.find("\n\n");
// 		pos += 2;
// 	}
// 	else 
// 		pos += 4;
// 	if (pos >= request.npos) {
// 		socket->setBody(body);
// 		return 0;
// 	}
// 	if (chunkedPos != std::string::npos)
// 		return httpRequestParseChunkedBody(request, socket, pos);

// 	try {
// 		body = request.substr(pos, request.npos);
// 		size_t	contentLength = atol(socket->getContentLength().c_str());
// 		if (body.size() >= contentLength)
// 			socket->setBody(body);
// 		else {
// 			char	c[8192];
			
// 			//Log::debug("\033[0;32mRead if body missing");
// 			while (body.size() < contentLength) {
// 				ft_bzero(c, 8192);
// 				ret = read(socket->getFd(), c, 8192);
// 				body.append(c, ret);
// 			}
// 			if (body.size() > contentLength)
// 				body.erase(contentLength, body.npos);
// 			socket->setBody(body);
// 		}
// 	}
// 	catch (std::exception &e) {
// 		std::cerr << "Exception2: " << e.what() << std::endl;
// 	}
	
// 	return 0;
// }

static int		httpRequestParseHeaders(std::string request, HttpRequest *socket) {
	char const *x[] = {
		"\nAccept-Charset",
		"\nAccept-Language",
		"\nAuthorization",
		"\nContent-Length",
		"\nContent-Location",
		"\nContent-Type",
		"\nDate",
		"\nHost",
		"\nReferer",
		"\nTransfer-Encoding",
		"\nUser-Agent",
		"\nX-Secret"
	};
	void (*f[])(HttpRequest*, std::string&, size_t) = {
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
		&ParseUserAgent,
		&ParseXSecret
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

static int		httpRequestParseRequestLine(std::string request, HttpRequest *socket) {
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

HttpRequest* HttpRequest::parseRequest(std::string &request) {

	HttpRequest *socket;

	socket = new HttpRequest();
	httpRequestParseRequestLine(request, socket);
	httpRequestParseHeaders(request, socket);
	// httpRequestParseBody(request, socket);
	// request.clear();
	return socket;
}



// bool         HttpRequest::extractChunks(BufferChain&, char**, size_t*)
// {

// }

int      HttpRequest::getBodyReceived()
{
	return _body_received;
}

void     HttpRequest::incBodyReceived(int value)
{
	_body_received += value;
}

bool         HttpRequest::extractBody(BufferChain& read_chain, BufferChain& stream_write_chain, HttpRequest* request)
{
	int diff;
	std::string *buff;

	// whle there are buffers to read or break
	while ((buff = read_chain.getFirst()) != NULL)
	{
		Log::debug("loop");
		// This is how many byte left do we need to read to have the full body
		diff = request->getContentLength() - request->getBodyReceived();
		// If the buffer doesn't contains the end of the body
		if (diff > (int)buff->size())
		{
			Log::debug("Reading all buffer");
			stream_write_chain.pushBack(buff);
			request->incBodyReceived(buff->size());
			read_chain.popFirst();
		}
		// else the buffer contains the last bytes needed
		else
		{
			Log::debug("Reading a part of the buffer");
			// copying the last part of the body into a buffer
			std::string* n = new std::string(buff->c_str(), diff);
			stream_write_chain.pushBack(n);
			request->incBodyReceived(diff);

			// if there's still some bytes left, append them to the read chain for later read
			if (diff < buff->size())
			{
				std::string* leftovers = new std::string(buff->c_str(), diff, buff->size() - diff);
				read_chain.pushBack(leftovers);
			}
			// Delete and remove the last buffer since it's been splitted
			delete buff;
			read_chain.popFirst();
			break;
		}
	}
	// If the body receveived match the content lenght we've read all the body
	if (request->getBodyReceived() == request->getContentLength())
		return true;
	return false;
}
