#include "Http.hpp"

Http::Http(Connection &c) :
_connection(c),
_read_chain(c.getReadChain()),
_write_chain(c.getWriteChain())
{
	_req = NULL;
	_resp = NULL;
	_config = NULL;
	_status_socket = WAITING_HTTP_HEADERS;
	_status_stream_read = NONE;
	_status_stream_write = NONE;
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
// The bytes have already been loaded into _read_chain by ths point
void Http::handleRead()
{
	if (_status_socket == WAITING_HTTP_HEADERS)
		handleNewRequest();
	if (_status_socket == WAITING_HTTP_BODY)
	{
		handleBodyRead();
		if (_stream_write_chain.getLast())
		{
			_bodyRec += _stream_write_chain.getLast()->size();
		}
		
	if (_status_socket == DONE && _req->getMethod() == "PUT")
			_connection.subWrite();
		// If there's something to write to stream
		if (_status_stream_write == WAITING_STREAM_WRITE && _stream_write_chain.getFirst())
			_connection.subStreamWrite();
	}
	checkState();
}

void Http::handleWrite()
{
	checkState();
}


void Http::handleStreamRead()
{
	int ret = -1;
	Log::debug("handleStreamRead()");

	if (_status_stream_read == WAITING_CGI_HEADERS) // TODO rather than doing all this checking you could just make a pointer to function
		handleCGIRead();
	else if (_resp->getTransferEncoding() == "chunked")
	{
		ret = readChunkToBuffer(_write_chain, _resp->getStreamRead());
		if (ret > 0)
			_bodySend += _write_chain.getLast()->size();
		
	}
	// Regular body, no formatting required
	else
	{
		try
		{
			ret = BufferChain::readToBuffer(_write_chain, _resp->getStreamRead());
		}
		catch(const std::exception& e)
		{
			throw;
		}
	}
	if (ret == 0)
	{
		_status_stream_read = DONE;
		_connection.unsubStreamRead();
		_status_stream_write = DONE;
		_connection.unsubStreamWrite();
	}
	if (_write_chain.getFirst())
		_connection.subWrite();
	checkState();
}

void Http::handleStreamWrite()
{
	int ret;

	try
	{
		ret = BufferChain::writeBufferToFd(_stream_write_chain, _resp->getStreamWrite());
		_bodyStreamWritten += ret;
		std::string* buff = _stream_write_chain.getFirst();
		if ((size_t) ret < buff->size())
		{
			std::string * leftovers = new std::string();
			leftovers->reserve(buff->size() - ret);
			leftovers->append(*buff, ret);
			delete buff;
			_stream_write_chain.popFirst();
			_stream_write_chain.pushFront(leftovers);
		}
		else
		{
			delete buff;
			_stream_write_chain.popFirst();
		}
	}
	catch(const std::exception& e)
	{
		throw;
	}
	if (_stream_write_chain.getFirst() == NULL)
	{
		_connection.unsubStreamWrite();
		// If there's no stream to read
		if (_status_socket == DONE)
		{
			_status_stream_write = DONE;
			close(_resp->getStreamWrite());
		}
	}
	checkState();
}

void	Http::checkState()
{
	std::cout << " _read_chain: " << _read_chain;
	std::cout << " _stream_write_chain: " << _stream_write_chain;
	std::cout << " _stream_read_chain: " << _stream_read_chain;
	std::cout << " _write_chain: " <<_write_chain;
	std::cout << "_status_socket: " << _status_socket <<std::endl;
	std::cout << "_status_stream_write: " << _status_stream_write <<std::endl;
	std::cout << "_status_stream_read: " << _status_stream_read <<std::endl;  
	std::cout << "================= Body received so far" << _bodyRec << std::endl;
	std::cout << "================= Body stream written so far" << _bodyStreamWritten << std::endl;
	std::cout << "================= Body send so far " << _bodySend << std::endl;

	if (_status_socket == DONE && 
		(_status_stream_write == NONE || _status_stream_write == DONE) &&
		(_status_stream_read == NONE || _status_stream_read == DONE))
		{
			// std::cout << "======================================= REQUEST END" << std::endl;
			reset();
			if (_read_chain.size() > 0)
				handleRead();
			
		}

	// if there's a buffer, try init a new request
}

void	Http::setConfig(Config* c)
{
	_config = c;
}

void	Http::reset()
{
		_connection.subRead();
		_connection.unsubStreamRead();
		_connection.unsubStreamWrite();
		if (_resp && _resp->getStreamWrite() != -1)
			close(_resp->getStreamWrite());
		if (_resp && _resp->getStreamRead() != -1)
			close(_resp->getStreamRead());
		_connection.setStreamRead(-1);
		_connection.setStreamWrite(-1);
		if (_req)
		{
			delete _req;
			_req = NULL;
		}
		if (_resp)
		{
			delete _resp;
			_resp = NULL;
		}
		_status_socket = WAITING_HTTP_HEADERS;
		_status_stream_read = NONE;
		_status_stream_write = NONE;
		_requestBuffer.clear();
		_streamBuffer.clear();
}

Http::~Http()
{
	reset();
	_connection.unsubRead();
}
