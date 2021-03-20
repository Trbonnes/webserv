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
		// std::cout << "================================== REQUEST START" << std::endl;
		std::string request;
		request.append(_requestBuffer, 0, pos + 1); // TO DO realloc good ?
		
		// Log::debug(request);
		// if pos is before that, then there's some body left
		if (pos < _requestBuffer.size() - 4)
		{
			std::string* leftovers = new std::string(_requestBuffer, pos + 4);
			_read_chain.pushFront(leftovers);
		}
		std::string tmp = _requestBuffer.substr(0, pos + 1);
		_requestBuffer.clear();
		// Instantiate new request
		_req = HttpRequest::parseRequest(request);
		// Instantiate a new response from that request
		// TO DO add try catch for errors
		// std::cout << _req->getPort() << " AND " << _req->getHost() << std::endl;
		_resp = HttpResponse::newResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()), _write_chain);	
		// Adding streams in select fd sets
		_connection.setStreamWrite(_resp->getStreamWriteFd());
		_connection.setStreamRead(_resp->getStreamReadFd());
	}
}
