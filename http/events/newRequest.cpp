#include "http/Http.hpp"

// Handle the new request and cut the body to be processed separately
void Http::handleNewRequest()
{
	std::string* buff = _read_chain.getFirst(); // OPTIMIMZATION strchr
	_requestBuffer.append(*buff);
	delete buff;
	_read_chain.popFirst();

	size_t pos;
	// If the end of headers are reached
	if ((pos = _requestBuffer.find("\r\n\r\n")) != _requestBuffer.npos)
	{
		std::string request;
		request.append(_requestBuffer, 0, pos);
		
		// if pos is before that, then there's some body left
		if (pos < _requestBuffer.size() - 4)
		{
			std::string* leftovers = new std::string(_requestBuffer, pos + 4);
			_read_chain.pushFront(leftovers);
		}
		_requestBuffer.clear();
		// Instantiate new request
		_req = HttpRequest::parseRequest(request);
		// Instantiate a new response from that request
		_resp = HttpResponse::newResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()), _write_chain);	
		// Adding streams in select fd sets
		_connection.setStreamWrite(_resp->getStreamWriteFd());
		_connection.setStreamRead(_resp->getStreamReadFd());
	}
}
