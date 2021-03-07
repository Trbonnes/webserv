#include "http/Http.hpp"

// Handle the new request and cut the body to be processed separately
void Http::handleNewRequest()
{
	std::string* buff = _read_chain.getFirst(); // TO DO might do a strrchr on bfufer chain to avoid multiple allocation
	_requestBuffer.append(*buff);
	delete buff;
	_read_chain.popFirst();


	size_t pos;
	// If the end of headers are reached
	if ((pos = _requestBuffer.find("\r\n\r\n")) != _requestBuffer.npos) // TO DO might have to hcange this for small buffers
	{








		// TO OD help thos poor souls
		_bodySend = 0;
		_bodyRec = 0;
		_bodyStreamWritten = 0;
		
		
		
		
		std::string request;
		request.append(_requestBuffer, 0, pos + 1); // TO DO realloc good ?
		
		// if pos is before that, then there's some body left
		if (pos < _requestBuffer.size() - 4)
		{
			std::string* leftovers = new std::string(_requestBuffer, pos + 4);
			_read_chain.pushFront(leftovers);
		}
		std::string tmp = _requestBuffer.substr(0, pos + 1);
		Log::debug(tmp);
		_requestBuffer.clear();
		// Instantiate new request
		_req = HttpRequest::parseRequest(request);
		// Instantiate a new response from that request
		_resp = new HttpResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()));
		
		// If the content length is superior to 0 or the body is chunked, then wait for the body
		if (_req->getContentLength() > 0 || _req->getTransferEncoding() == "chunked\r") // TO DO why ?
			_status_socket = WAITING_HTTP_BODY;
		else
		{
			// else we can wait for the end of the response
			_connection.unsubRead();
			_status_socket = DONE;
		}
		// Adding streams in select fd sets
		// By this point the Response should have fd's for CGI or a regular file
		// if stream write
		// CGI and PUT
		if (_resp->getStreamWrite() != -1)
		{
			_status_stream_write = WAITING_STREAM_WRITE;
			_connection.setStreamWrite(_resp->getStreamWrite());
		}
		//if stream read
		// CGI and GET
		if (_resp->getStreamRead() != -1)
		{
			_status_stream_read = WAITING_STREAM_READ;
			_connection.setStreamRead(_resp->getStreamRead());
			_connection.subStreamRead();
		}

		// if the request use cgi we might have to get others http headers from it
		if (_resp->use_cgi())
		{
			std::cout << "TIS WLL UE SE -------------------------" << std::endl;	
			_status_stream_read = WAITING_CGI_HEADERS;
		}
		else
		{
			// Getting headers of response
			buff = _resp->getHeaders();
			// Getting the headers and eventually the full response
			_write_chain.pushBack(buff);

			// Subbing for write
			if (_req->getMethod() != "PUT")
				_connection.subWrite();
			// If body is true append body
			buff = _resp->getBody();
			if (buff)
				_write_chain.pushBack(buff);
		}
	}
}
