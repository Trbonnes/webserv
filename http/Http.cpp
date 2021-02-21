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
	// if null then headers are not fully received
	if (_req == NULL)
	{
		char* needle;
		BufferChain::buffer_t* last = _read_chain.getLast();

		// If the end of headers are reached
		if ((needle = ft_strnstr(last->data, "\r\n\r\n", last->size)))
		{
			std::string request("");

			// Concatenate headers
			BufferChain::buffer_t* curr = _read_chain.getFirst();
			while (curr != last)
			{
				request.append(curr->data, curr->size);
				_read_chain.popFirst();
				delete[] curr->data;
				curr = _read_chain.getFirst();
			}
			// Concatenate last buffer
			unsigned int diff = needle - curr->data + 1;
			if (diff > 0)
				request.append(curr->data, diff);
			// check if some body is left
			if (diff + 4 < curr->size)
			{
				_stream_write_chain.copyPushBack(curr->data + diff + 4, curr->size - (diff + 4));
			}
			_read_chain.popFirst();
			delete[] curr->data;
			
			// Launch http parsing on newly formed request
			_req = HttpRequest::parseRequest(request);
			Log::debug(_req->getContentLength());

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
	// if (_rep != NULL)
	// {
	// 	// If all the body has been read, just ignore those packets
	// 	if (bodysize > _req->getContentLength())
	{
		_connection.subWrite();
		return;
	}


	// 	// Here the headers have been received, but not the entire body
	// 	// If it is chunked, we need to get rid of them
	// 	if (_req->getTransferEncoding() == "chunked")
	// 	{
	// 		HttpRequest::parseChunks(_read_chain, _stream_write_chain);
	// 	}
	// 	// else proceed as usual
	// 	else
	// 	{
	// 		HttpRequest::parseBody(_read_chain, _stream_write_chain);
	// 		// Getting the last body buffer read on this chain
	// 		BufferChain::buffer_t *buff = _read_chain.getFirst();

	// 		_stream_write_chain.pushBack(buff->data, buff->size);
	// 		_read_chain.popFirst();
	// 	}

	// 	// If the body is to be ignored, flush the stream's input buffer
	// 	if (ignore_body)
	// 		_stream_write_chain.flush();
	// 	// else stream the fd
	// 	else
	// 		_connection.subStreamWrite();
	// }
}

void Http::handleStreamRead()
{
	// if (BufferChain::readToBuffer(_stream_read_chain, _rep->getStreamOut()) == 0)
	// {
	// 	if (_req->getTransferEncoding() == "chunked")
	// 	{
	// 		HttpRequest::parseChunks(_read_chain, _stream_write_chain);
	// 	}
	// 	else
	// 	{

	// 	}
	// }
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
