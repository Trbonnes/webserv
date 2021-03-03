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
	_status_stream = DONE;
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
		// If the end of headers are reached
		if ((pos = _requestBuffer.find("\r\n\r\n")) != _requestBuffer.npos) // TO DO might have to hcange this for small buffers
		{
			std::string request;
			request.append(_requestBuffer, 0, pos + 1); // TO DO realloc good ?
			if (pos < _requestBuffer.size() - 4)
			{
				std::string* leftovers = new std::string(_requestBuffer, pos + 4, _requestBuffer.size() - pos - 4);
				_read_chain.pushFront(leftovers);
			}
			_requestBuffer.clear();
			// Instantiate new request
			_req = HttpRequest::parseRequest(request);
			// Instantiate a new response from that request
			_resp = new HttpResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()));




			
			if (_req->getContentLength() > 0 || _req->getTransferEncoding() == "chunked\r") // TO DO why ?
				_status_socket = WAITING_BODY;
			else
			{
				_connection.unsubRead();
				_status_socket = DONE;
			}
			// Adding streams in select fd sets
			// By this point the Response should have fd's for CGI or a regular file
			// if stream in
			// CGI and PUT
			if (_resp->getStreamWrite() != -1)
			{
				std::cout << "Stream write active !" << std::endl;
				_status_stream = ACTIVE;
				_connection.setStreamWrite(_resp->getStreamWrite());
			}
			//if stream out
			// CGI and GET
			if (_resp->getStreamRead() != -1)
			{
				_status_stream = ACTIVE;
				_connection.setStreamRead(_resp->getStreamRead());
			}

			// if the request use cg iwe might have to get others http headers from it, we can'
			if (_resp->use_cgi() == false)
			{
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
			}
			else
				_status_stream = WAITING_CGI_HEADERS;
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
	if (_resp->getStreamWrite() != -1 && _stream_write_chain.getFirst() != NULL)
	{
		std::cout << "SUBBING STREAMWRITE" << std::endl;
		_connection.subStreamWrite();
	}
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

	if (_status_socket == DONE && _status_stream == DONE)
		reset();
}


void Http::handleCGIRead()
{
	int ret;
	size_t pos;

	if (_status_stream == WAITING_CGI_HEADERS)
	{
		try
		{
			ret = BufferChain::readToBuffer(_stream_read_chain, _resp->getStreamRead());
			if (ret == 0)
				reset();
		}
		catch(const std::exception& e)
		{
			throw;
		}
		if ((pos = _stream_read_chain.getFirst()->find("\r\n\r\n")) != std::string::npos) // TO DO add join mechanic if buffer is incomplete
		{
			std::string *headers = new std::string();
			headers->reserve(pos + 4);
			headers->append(*_stream_read_chain.getFirst(), 0, pos);
			headers->append("\r\n\r\n");

			std::string *body = chunkify((char*)_stream_read_chain.getFirst()->c_str(), pos + 4 , _stream_read_chain.getFirst()->size() - pos - 4);
			_write_chain.pushBack(_resp->getHeaders()); 
			_write_chain.pushBack(headers); 
			_write_chain.pushBack(body);
			std::cout << _write_chain;
			delete _stream_read_chain.getFirst();
			_stream_read_chain.popFirst();
			_status_stream = ACTIVE;
			
			// dont forget to like and subwrite
			_connection.subWrite();
		}
	}
}

void Http::handleStreamRead()
{
	int ret;
	Log::debug("handleStreamRead()");

	if (_status_stream == WAITING_CGI_HEADERS) // TODO rather than doing all this checking you could just make a pointer to function
		handleCGIRead();
	else if (_resp->getTransferEncoding() == "chunked")
	{
		ret = readChunkToBuffer(_write_chain, _resp->getStreamRead());
		if (ret == 0)
			reset();
	}
	// Regular body, no formatting required
	else
	{
		try
		{
			ret = BufferChain::readToBuffer(_write_chain, _resp->getStreamRead());
			if (ret == 0)
				reset();
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
	try
	{
		BufferChain::writeBufferToFd(_stream_write_chain, _resp->getStreamWrite());
		std::string* buff = _stream_write_chain.getFirst();
		delete buff;
		_stream_write_chain.popFirst();
	}
	catch(const std::exception& e)
	{
		throw;
	}
	
	if (_stream_write_chain.getFirst() == NULL)
	{
		_connection.unsubStreamWrite();
		// if all the body i s received, then we don't have anything more to do
		if (_status_socket == DONE)
			close(_resp->getStreamWrite());
	}
	_connection.subStreamRead();
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

void	Http::reset()
{
		std::cout << "REQUEST END" << std::endl;
		destroyRequest();
		destroyResponse();
		_connection.subRead();
		_connection.unsubStreamRead();
		_connection.unsubStreamWrite();
		_status_socket = WAITING_HEADERS;
		_status_stream = DONE;
}

std::string*	Http::chunkify(char* buff, size_t start, size_t len)
{
	std::cout << "============ Chunkyfying ==============" << std::endl;
	//converting to hex
	std::stringstream ss;
	ss << std::hex << len;
	std::string* n = new std::string();
	std::string chunkstr = ss.str();

	// reserving the space needed
	n->reserve(chunkstr.size() + 2 + len + 2);

	// appending the chunk
	n->append(chunkstr);
	n->append("\r\n");
	if(len != 0)
		n->append(buff, start, len);
	n->append("\r\n");
	std::cout << "chunkstr: " << chunkstr << " len: " <<  len << std::endl;
	std::cout << "chunkstr: " << chunkstr << " len: " <<  len << std::endl;
	return n;
}


int		Http::readChunkToBuffer(BufferChain& chain, FD fd)
{
	int ret;
	std::string *n;

	ret = read(fd, g_read_large, BUFFER_SIZE_LARGE); // TO DO keep this size ?
	if (ret == -1)
		throw IOError();
	n = chunkify(g_read_large, 0, ret);
	chain.pushBack(n);
	return ret;
}
