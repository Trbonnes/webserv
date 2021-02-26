#include "Http.hpp"

Http::Http(Connection &c) :
_connection(c),
_read_chain(c.getReadChain()),
_write_chain(c.getWriteChain())
{
	_req = NULL;
	_resp = NULL;
	_config = NULL;
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



// This methods is called each time there's a socket read
// The bytes have already been loaded into _read_cahin by ths point
void Http::handleRead()
{
	Log::debug("handleRead()");
	// if null then headers are not fully received
	if (_req == NULL)
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

			// Getting headers of response
			buff = _resp->getHeaders();
			_connection.subWrite();
			_connection.unsubRead();
			// Getting the headers and eventually the full response
			_write_chain.pushBack(buff);
			// If all are minus one then there's nothing to read / write, the request is done

			if (_resp->getStreamRead() == -1 && _resp->getStreamWrite() == -1)
			{
				buff = _resp->getBody();
				if (buff)
					_write_chain.pushBack(buff);
				return;
			}


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
	// if it's not NUll then we have work to do
	// it's not in an else statement, cause the above if statement will have some body leftovers in the read chain
	if (_resp != NULL && _read_chain.getFirst() != NULL)
	{
		Log::debug("Reading body");
		Log::debug(_req->getTransferEncoding());
		bool	end = false;

		if (_req->getTransferEncoding() == "chunked\r")
		{
			try
			{
				end = HttpRequest::extractChunks(_read_chain, _stream_write_chain);
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
			Log::debug("End of read");
			// unsub read so we pause the read process
			_connection.unsubRead();
		}
		std::cout << _stream_write_chain;
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
				_connection.unsubStreamRead();
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
