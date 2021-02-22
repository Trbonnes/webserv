/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2021/02/22 16:33:15 by yorn             ###   ########.fr       */
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


static void cutLeftovers(BufferChain& chain, std::string* curr, size_t hexStart)
{
	std::string* leftovers;
	if (hexStart > 0)
	{
		leftovers = new std::string(*curr, hexStart);
		if (leftovers->size() == 0)
			delete leftovers;
		else
			chain.pushFront(leftovers);
		delete curr;
	}
	else
	{
		chain.pushFront(curr);
	}
}

// TO DO might optimize this
static void appendNext(BufferChain& chain, std::string* curr)
{
	std::string *first;

	first = chain.getFirst();
	curr->append(*first);
	delete first;
	chain.popFirst();
	chain.pushFront(curr);
}

static int appendChunk(BufferChain& chain, std::string* dst, size_t *offset)
{
	size_t hexStart = *offset;
	size_t endLen;

	// popping the current buffer cause it's gonna be split
	std::string *buff = chain.getFirst();
	chain.popFirst();

	endLen = buff->find("\r\n", hexStart);
	// No symbol found
	if (endLen == buff->npos)
	{
		// if the next buffer is not available, cut the current one
		if (chain.getFirst() == NULL)
		{
			cutLeftovers(chain, buff, hexStart);
			*offset = 0;
			return 0;
		}
		// if the next buffer is available append it to the actual buffer
		else
		{
			appendNext(chain, buff);
			*offset = 0;
			return 1;
		}
	}
	// If this is true the lenght string is non existent
	if (hexStart == endLen)
		return -1;
	std::string strLen(*buff, hexStart, endLen);
	size_t bodyLen;
	char* endptr;
	bodyLen = std::strtol(strLen.c_str(), &endptr, 16);
	// if it's not all in base 16
	if ((unsigned long)(endptr - strLen.c_str()) != endLen - hexStart)
		return -1;
	// if all the body isn't in the chunk
	size_t bodyStart = endLen + 2;
	size_t bodyEnd = bodyStart + bodyLen + 2;
	if (bodyEnd > buff->size())
	{
		// if the next buffer is not available, cut the current one
		if (chain.getFirst() == NULL)
		{
			cutLeftovers(chain, buff, hexStart);
			*offset = 0;
			return 0;
		}
		// if the next buffer is available append it to the actual buffer
		else
		{
			appendNext(chain, buff);
			*offset = 0;
			return 1;
		}
	}
	if (bodyLen == 0)
	{
		cutLeftovers(chain, buff, bodyEnd);
		return 2;
	}
	
	dst->append(*buff, bodyStart, bodyLen);
	*offset = bodyEnd;
	chain.pushFront(buff);
	return (1);
}

// Extract chunks into a regular body
bool         HttpRequest::extractChunks(BufferChain& read_chain, BufferChain& stream_write_chain)
{ // TO DO optimizations

	size_t offset = 0;
	int r;
	std::string* newBuffer = new std::string("");

	offset = 0;
	while ((r = appendChunk(read_chain, newBuffer, &offset)) == 1)
	{
		std::cout << "|" << *newBuffer <<  "|" << std::endl;
		// std::cout << "|" << *read_chain.getFirst() <<  "|" << std::endl;
		continue;
	}
	// if -1 error
	if (r == -1)
		throw HttpRequest::MalformedChunk();

	// If there's something to write do it. else rease the buffer
	if (newBuffer->size() > 0)
		stream_write_chain.pushBack(newBuffer);
	else 
		delete newBuffer;
	// if 0 need more to read
	if (r == 2)
		return true;
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
