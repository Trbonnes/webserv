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
		std::string* last = _read_chain.getLast();
		std::string* buff;
		size_t needle;
		// If the end of headers are reached
		if ((needle = last->find("\r\n\r\n")) != last->npos) // TO DO might have to hcange this for small buffers
		{
			Log::debug("End of headers reached !");
			size_t space = 0;

			std::string request("");
			if (_read_chain.size() > 1) // TO DO ugly
			{
				// Calculating space to reserve
				for (BufferChain::iterator it = _read_chain.begin(); *it != last; it++)
					space += (*it)->size();
				space += needle;	
				// reserveing the space
				request.reserve(space);
				
				// appending the data
				while ((buff = _read_chain.getFirst()) != last)
				{
					request.append(*buff);
					delete buff;
					_read_chain.popFirst();
				}
				request.append(*buff, needle);
				
				// If there's leftovers, append them to the read chain to be processed as body or else
				size_t offset = needle + 4;
				if (offset < buff->size())
				{
					std::string* leftovers = new std::string(buff->c_str(), offset, buff->size() - offset);
					_read_chain.pushBack(leftovers);
				}
				// then delete the current buffer
				delete buff;
				_read_chain.popFirst();
			}
			else
			{
				buff = _read_chain.getFirst();
				request.append(*buff);
				delete buff;
				_read_chain.popFirst();
			}

			// Instantiate new request
			_req = HttpRequest::parseRequest(request);
			// Instantiate a new response from that request
			_resp = new HttpResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()));

			// Getting headers of response
			buff = _resp->getHeaders();
			Log::debug(*buff);
			_connection.subWrite();
			// Getting the headers and eventually the full response
			_write_chain.pushBack(buff);
			// If all are minus one then there's nothing to read / write, the request is done
			if (_resp->getStreamIn() == -1 && _resp->getStreamIn() == -1)
			{
				buff = _resp->getBody();
				std::cout << "buff: |" << *buff << "|" <<  std::endl;
				if (buff)
					_write_chain.pushBack(buff);
				std::cout << "SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIZE: " << _write_chain.size() << std::endl;
				return;
			}

			// Adding streams in select fd sets
			// By this point the Response should have fd's for CGI or a regular file
			// if stream in
			// CGI and PUT
			if (_resp->getStreamIn() != -1)
			{
				_connection.setStreamWrite(_resp->getStreamIn());
			}
			//if stream out
			// CGI and GET
			if (_resp->getStreamOut() != -1)
			{
				_connection.setStreamRead(_resp->getStreamOut());
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
		
		// std::cout << "Transfer encoding:"  << std::endl;
		// std::cout << "|" << _req->getTransferEncoding() << "|" << std::endl;
		// std::cout << "|" << "chunked" << "|" << std::endl;
		// std::cout << "|" << (_req->getTransferEncoding() == "chunked") << "|" << std::endl;
		// std::cout << "|" << (_req->getTransferEncoding().compare("chunked")) << "|" << std::endl;
		// If it is chunked we need to extract those into a new buffer
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
	if (_resp->getTransferEncoding() == "chunked")
	{
		// HttpResponse::
	}
	// Regular body, no formatting required
	else
	{
		try
		{
			BufferChain::readToBuffer(_write_chain, _resp->getStreamIn());
		}
		catch(const std::exception& e)
		{
			throw;
		}
	}
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
