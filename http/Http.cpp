#include "Http.hpp"

Http::Http(Connection &c) :
_connection(c),
_read_chain(c.getReadChain()),
_write_chain(c.getWriteChain())
{
	_req = NULL;
	_resp = NULL;
	_config = NULL;
	_status_socket = WAITING_HEADERS;
}

Http::Http(const Http &c) :
_connection(c._connection),
_read_chain(c._connection.getReadChain()),
_write_chain(c._connection.getWriteChain())
{
	(void) c; // TO DO implement copy and operator=
}

Http& Http::operator=(const Http &c)
{
	(void) c; // TO DO implement copy and operator=
	return *this;
}

void Http::handleNewRequest()
{
		std::string* buff = _read_chain.getFirst(); // TO DO might do a strrchr on bfufer chain to avoid multiple allocation
		_requestBuffer.append(*buff);
		delete buff;
		_read_chain.popFirst();
		

		size_t pos;
		std::cout << _requestBuffer << std::endl;
		// If the end of headers are reached
		if ((pos = _requestBuffer.find("\r\n\r\n")) != _requestBuffer.npos) // TO DO might have to hcange this for small buffers
		{
			std::cout << "New request " << std::endl;
			std::string request;
			request.append(_requestBuffer, 0, pos + 1); // TO DO realloc good ?
			if (pos < _requestBuffer.size() - 5)
			{
				std::string* leftovers = new std::string(_requestBuffer, pos + 4, _requestBuffer.size() - pos - 5);
				_read_chain.pushFront(leftovers);
			}
			_requestBuffer.clear();
			// Instantiate new request
			_req = HttpRequest::parseRequest(request);
			// Instantiate a new response from that request
			_resp = new HttpResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()));

			// Subbing
			_connection.subWrite();

			// Getting headers of response
			buff = _resp->getHeaders();
			// Getting the headers and eventually the full response
			_write_chain.pushBack(buff);
			// If body is true append body
			buff = _resp->getBody();
			if (buff)
				_write_chain.pushBack(buff);
			
			if (_req->getContentLength() > 0 || _req->getTransferEncoding() == "chunked\r") // TO DO why ?
			{
				std::cout << "We gotta wait some body" << std::endl;
				_status_socket = WAITING_BODY;
			}
			else
				_status_socket = DONE;
			// Adding streams in select fd sets
			// By this point the Response should have fd's for CGI or a regular file
			// if stream in
			// CGI and PUT
			if (_resp->getStreamWrite() != -1)
			{
				_connection.setStreamWrite(_resp->getStreamWrite());
				_connection.subStreamWrite();	
			}
			//if stream out
			// CGI and GET
			if (_resp->getStreamRead() != -1)
			{
				_connection.setStreamRead(_resp->getStreamRead());
				_connection.subStreamRead();
			}
		}
}

void Http::handleBodyRead()
{
	bool	end = false;

	if (_req->getTransferEncoding() == "chunked\r") // TO DO why the f do i have to add \r
	{
		try
		{
			end = HttpRequest::extractChunks(_read_chain, _stream_write_chain); // TO DO is it ugly ? II think so
			std::cout << end << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			// TO DO set error bad request in response status
		}
	}
	else
	{
		end = HttpRequest::extractBody(_read_chain, _stream_write_chain, _req);
	}
	// if end then all body has been received
	if (end)
	{
		Log::debug("All the body has been read");
		if (_resp->getStreamWrite() == -1)
			_stream_write_chain.flush();
		_status_socket = DONE;
	}
	std::cout << _stream_write_chain;
}

// This methods is called each time there's a socket read
// The bytes have already been loaded into _read_cahin by ths point
void Http::handleRead()
{
	Log::debug("handleRead()");	
	if (_status_socket == WAITING_HEADERS)
		handleNewRequest();

	if (_status_socket == WAITING_BODY)
		handleBodyRead();

	if (_status_socket == DONE)
	{
		std::cout << "REQUEST END" << std::endl;
		destroyRequest();
		destroyResponse();
		_status_socket = WAITING_HEADERS;
	}
}

void Http::handleStreamRead()
{
	int ret = -1;
	Log::debug("handleStreamRead()");
	if (_resp->getTransferEncoding() == "chunked")
	{
		// HttpResponse::
	}
	// Regular body, no formatting required
	else
	{
		try
		{
			ret = BufferChain::readToBuffer(_write_chain, _resp->getStreamRead());
			if (ret == 0)
			{
				destroyResponse();
				destroyRequest();
				_connection.subRead();
				_connection.unsubStreamRead();
			}
		}
		catch(const std::exception& e)
		{
			throw;
		}
	}
	if (_write_chain.getFirst())
		_connection.subWrite();
}

void Http::handleStreamWrite()
{
	// char* buff = NULL;
	// size_t read;

	// // If read buffer are supposed to get chunked out
	// if (_rep->getTransferEncoding() == "chunked")
	// {
	// 	read = _rep->parseChunk(&buff);
	// }
	// else
	// {
	// 	read = _rep->parseBody(&buff);
	// }
	// // if EOF
	// if (read == 0)
	// {
	// 	_connection.unsubStreamRead();
	// 	return;
	// }
	// _write_chain.pushBack(buff, read);
	// TO DO can we really have a larger buffer chain than content length ?
}

void	Http::setConfig(Config* c)
{
	_config = c;
}

Http::~Http()
{
	if (_req)
		delete _req;
	if (_resp)
		delete _resp;
}

void	Http::destroyRequest()
{
	if (_req)
	{
		delete _req;
		_req = NULL;
	}
}

void	Http::destroyResponse()
{
	if (_resp)
	{
		delete _resp;
		_resp = NULL;
	}
}