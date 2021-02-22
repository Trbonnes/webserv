/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/22 14:11:56 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

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



// Extract chunks into a regular body
bool         HttpRequest::extractChunks(BufferChain& read_chain, BufferChain& stream_write_chain, HttpRequest* request)
{ // TO DO optimizations

	std::string* buff = new std::string("");
	std::string* curr;
	size_t	 	 bodylen;
	size_t startLen = 0;
	size_t endLen;
	size_t startBody;
	size_t len;
	
	while ((curr = read_chain.getFirst()) != NULL)
	{

		// Find an end for chunk sizze
		end = curr->find("\r\n");
		// if no match
		if (end == buff->npos)
		{
			// return false if last buffer, and wait for the next read
			if (curr == read_chain.getLast())
				return false;
			// esle concatenate with the next and relaunch the loop
			else
			{
				read_chain.popFirst();
				curr->append(*read_chain.getFirst());
				delete read_chain.getFirst();
				read_chain.popFirst();
				// we're pushing it on the front so the loop takes it
				read_chain.pushFront(curr);
				continue;
			}
		}
		

		size_t numberEnd;
		std::string tmp = buff->substr(startLen, buff->size - endLen);
		bodylen = std::stoul(tmp, &numberEnd, 16);
		// if the number doesn't span all the string
		if (tmp.size() == 0 || startLen + numberEnd != endLen)
			throw HttpRequest::MalformedChunk();
		if (bodylen == 0)
		{

		}

	}
	return false;
}

int      HttpRequest::getBodyReceived()
{
	return _body_received;
}

void     HttpRequest::incBodyReceived(int value)
{
	_body_received += value;
}

// Extract regular body
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
			if (diff < (int)buff->size())
			{
				std::string* leftovers = new std::string(buff->c_str(), diff, buff->size() - diff);
				read_chain.pushFront(leftovers);
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
