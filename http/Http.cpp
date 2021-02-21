#include "Http.hpp"

Http::Http(Connection &c) :
_connection(c),
_read_chain(c.getReadChain()),
_write_chain(c.getWriteChain())
{
	_req = NULL;
	_rep = NULL;
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
		char* needle;
		BufferChain::buffer_t* last = _read_chain.getLast();

		// If the end of headers are reached
		if ((needle = ft_strnstr(last->data, "\r\n\r\n", last->size)))
		{
			Log::debug("End of headers reached !");
			std::string request("");

			// Concatenate headers
			BufferChain::buffer_t* curr = _read_chain.getFirst();
			while (curr != last)
			{
				request.append(curr->data, curr->size);
				delete[] curr->data;
				_read_chain.popFirst();
				curr = _read_chain.getFirst();
			}
			// Concatenate last buffer
			unsigned int diff = needle - curr->data + 1;
			if (diff > 0)
				request.append(curr->data, diff);
			// Launch http parsing on newly formed request
			_req = HttpRequest::parseRequest(request);
			// check if some body is left
			if (diff + 4 < curr->size)
			{
				_read_chain.copyPushBack(curr->data + diff + 4, curr->size - (diff + 4));
			}
			delete[] curr->data;
			_read_chain.popFirst();
			
			// Instantiate a new response
			_rep = new HttpResponse(_req, _config->getServerUnit(_req->getPort(), _req->getHost()));
			

			// if (_rep->use_cgi() || _req->getTransferEncoding() == "chunked" || FIchier trop lourd)
			// {
			// 	_rep.set
			// }

			// Adding streams in select fd sets
			// By this point the Response should have fd's for CGI or a regular file
			// if stream in
			// CGI and PUT
			// if (_rep->getStreamIn() != -1)
			// {
			// 	_connection.setStreamWrite(_rep->getStreamIn());
			// }
			// //if stream out
			// // CGI and GET
			// if (_rep->getStreamOut() != -1)
			// {
			// 	_connection.setStreamRead(_rep->getStreamOut());
			// }
		}
	}
	// if it's not NUll then we have work to do
	// it's not in an else statement, cause the above if statement will have some body leftovers in the read chain
	if (_rep != NULL && _read_chain.getFirst() != NULL)
	{
		Log::debug("Reading body");
		char *target = NULL;
		size_t size = 0;
		bool	end = false;
		// If it is chunked we need to extract those into a new buffer
		if (_req->getTransferEncoding() == "chunked")
		{
			// Extract the chunks for the curent request
			// end = HttpRequest::extractChunks(_read_chain, &target, &size);	
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
			std::cout << _stream_write_chain;
			_connection.unsubRead();
		}
		if (target)
			_stream_write_chain.pushBack(target, size);
		// if (ignore_body)
		// 	_stream_write_chain.flush();
	}
}

void Http::handleStreamRead()
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

void Http::handleStreamWrite()
{
	// Load he response into the write fchaine
	// BufferChain::writeBufferToFd(_stream_write, _rep->getStreamIn());
}

void	Http::setConfig(Config* c)
{
	_config = c;
}

Http::~Http()
{
	if (_req)
		delete _req;
	if (_rep)
		delete _rep;
}
