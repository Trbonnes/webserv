#include "http/Http.hpp"


void Http::handleCGIRead()
{
	int ret;
	size_t pos;

	if (_status_stream_read == WAITING_CGI_HEADERS)
	{
		try
		{
			ret = BufferChain::readToBuffer(_stream_read_chain, _resp->getStreamRead());
			if (ret == 0)
			{
				_connection.unsubStreamRead();
				_status_stream_read = DONE;
				return;
			}
			_streamBuffer.append(*_stream_read_chain.getFirst());
			delete _stream_read_chain.getFirst(); // TODO not the most optimized
			_stream_read_chain.popFirst();
		}
		catch(const std::exception& e)
		{
			throw;
		}
		if ((pos = _streamBuffer.find("\r\n\r\n")) != std::string::npos) // TO DO add join mechanic if buffer is incomplete
		{
			std::string *headers = new std::string();
			headers->reserve(pos + 4);
			headers->append(_streamBuffer, 0, pos);
			headers->append("\r\n\r\n");
			_write_chain.pushBack(_resp->getHeaders());
			_write_chain.pushBack(headers);
			std::string *body = chunkify((char*)_streamBuffer.c_str(), pos + 4 , _streamBuffer.size() - pos - 4);
			_write_chain.pushBack(body);
			_status_stream_read = WAITING_STREAM_READ;
			_streamBuffer.clear();
			// dont forget to like and subwrite
			_connection.subWrite();
		}
	}
}