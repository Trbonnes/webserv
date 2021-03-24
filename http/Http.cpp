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
// The bytes have already been loaded into _read_chain by ths point
void Http::handleRead()
{
	if (_resp == NULL)
		handleNewRequest();
	if (_resp && _read_chain.getFirst())
	{
		try
		{
			_resp->handleRead(_read_chain, _write_chain);
		}
		catch(const HttpResponse::HttpError& e)
		{
			handleHttpError(e.getStatusCode());
		}
		catch(const HttpResponse::ConnectionClose& e)
		{
			// std::cout << "COnnection is closed by exception" << std::endl;
			throw;
		}
	}
	if (_resp)
		checkState();
}

void Http::handleWrite()
{
	try
	{
		if (_resp)
			_resp->handleWrite(_read_chain, _write_chain);
	}
	catch(const HttpResponse::HttpError& e)
	{
		handleHttpError(e.getStatusCode());
	}
	catch(const HttpResponse::ConnectionClose& e)
	{
		// std::cout << "COnnection is closed by exception" << std::endl;
		throw;
	}
	checkState();
}


void Http::handleStreamRead()
{
	// Log::debug("handleStreamRead()");
	try
	{
		_resp->handleStreamRead(_read_chain, _write_chain);
	}
	catch(const HttpResponse::HttpError& e)
	{
		handleHttpError(e.getStatusCode());
	}
	catch(const HttpResponse::ConnectionClose& e)
	{
		// std::cout << "COnnection is closed by exception" << std::endl;
		throw;
	}
	checkState();
}

void Http::handleStreamWrite()
{
	try
	{
		_resp->handleStreamWrite(_read_chain, _write_chain);
	}
	catch(const HttpResponse::HttpError& e)
	{
		handleHttpError(e.getStatusCode());
	}
	catch(const HttpResponse::ConnectionClose& e)
	{
		// std::cout << "COnnection is closed by exception" << std::endl;
		throw;
	}
	checkState();
}

void Http::handleHttpError(int statusCode)
{


	std::cout << "==================================================== handleHttpError" << std::endl;
	std::cout << " _read_chain: " << _read_chain;
	if (_resp)
	{
		std::cout << " _stream_write_chain: " << _resp->getStreamWriteChain();
		std::cout << " _stream_read_chain: " << _resp->getStreamReadChain();
	}
	std::cout << " _write_chain: " <<_write_chain;


	if (_resp)
	{
		char* states[5];

		states[HttpResponse::NONE] = (char*)"NONE";
		states[HttpResponse::WAITING] = (char*)"WAITING";
		states[HttpResponse::IGNORE] = (char*)"IGNORE";
		states[HttpResponse::READY] = (char*)"READY";
		states[HttpResponse::DONE] = (char*)"DONE"; 

		std::cout << getpid() << " status _read: " << states[_resp->_state.read]
			<< " status _stream_write: " << states[_resp->_state.writeStream] 
			<< " status _stream_read: " << states[_resp->_state.readStream]
			<< " status _write: " << states[_resp->_state.write] << std::endl;
	}
		_write_chain.flush();
		_read_chain.flush();
		_connection.unsubStreamRead();
		_connection.unsubStreamWrite();
		HttpResponse* n = new Error(_resp->getConfig(), _req, _resp->getRoute(), _resp->getLocation(), _write_chain, statusCode);
		_connection.setStreamRead(n->getStreamReadFd());
		if (n->getStreamReadFd() != -1)
			_connection.subStreamRead();
		if (_resp->_state.read == HttpResponse::DONE)
			n->_state.read = HttpResponse::DONE;
		_resp->abort();
		delete _resp;
		_resp = n;
		
		_connection.setStreamRead(_resp->getStreamReadFd());
		_connection.setStreamWrite(_resp->getStreamWriteFd());

}

void	Http::checkState()
{
	std::cout << "---------------------- STATE" << std::endl;
	
	if (_resp)
	{
		if (_resp->_state.read == HttpResponse::READY)
			_connection.subRead();
		else
			_connection.unsubRead();
		if (_resp->_state.readStream == HttpResponse::READY)
			_connection.subStreamRead();
		else
		{
			_connection.unsubStreamRead();
			if (_resp->_state.readStream == HttpResponse::DONE)
				_connection.setStreamRead(-1);
		}
		if (_resp->_state.writeStream == HttpResponse::READY)
			_connection.subStreamWrite();
		else
		{
			_connection.unsubStreamWrite();
			if (_resp->_state.writeStream == HttpResponse::DONE)
			{
				// std::cout << "Closing " << _resp->getStreamWriteFd() << std::endl;
   			close(_resp->getStreamWriteFd());
				_resp->setStreamWriteFd(-1);
				_connection.setStreamWrite(-1);
			}
		}
		
		if (_write_chain.getFirst())
			_connection.subWrite();
		else
			_connection.unsubWrite();
	std::cout << " _read_chain: " << _read_chain;
	if (_resp)
	{
		std::cout << " _stream_write_chain: " << _resp->getStreamWriteChain();
		std::cout << " _stream_read_chain: " << _resp->getStreamReadChain();
	}
	std::cout << " _write_chain: " <<_write_chain;


	if (_resp)
	{
		char* states[5];

		states[HttpResponse::NONE] = (char*)"NONE";
		states[HttpResponse::WAITING] = (char*)"WAITING";
		states[HttpResponse::IGNORE] = (char*)"IGNORE";
		states[HttpResponse::READY] = (char*)"READY";
		states[HttpResponse::DONE] = (char*)"DONE"; 

		std::cout << getpid() << " status _read: " << states[_resp->_state.read]
			<< " status _stream_write: " << states[_resp->_state.writeStream] 
			<< " status _stream_read: " << states[_resp->_state.readStream]
			<< " status _write: " << states[_resp->_state.write] << std::endl;
	}
		// End of request
		if (_resp->_state.read == HttpResponse::DONE && _write_chain.getFirst() == NULL && 
			(_resp->_state.readStream == HttpResponse::DONE || _resp->_state.readStream == HttpResponse::NONE) &&
			(_resp->_state.writeStream == HttpResponse::DONE || _resp->_state.writeStream == HttpResponse::NONE))
		{
			// std::cout << "-------------- Request End " << &_connection << std::endl;
			reset();
			if (_read_chain.size() > 0)
				handleRead();
		}
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
		_requestBuffer.clear();
		_streamBuffer.clear();
}

Http::~Http()
{
	reset();
	_connection.unsubRead();
}
