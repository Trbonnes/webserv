// #include "http/Http.hpp"

// // reads the body
// void Http::	handleBodyRead()
// {
// 	bool	end = false;

// 	std::cout << _read_chain ;
// 	if (_req->getTransferEncoding() == "chunked\r") // TO DO why the f do i have to add \r
// 	{
// 		Log::debug("CHUNKING");
// 		try
// 		{
// 			end = HttpRequest::extractChunks(_read_chain, _stream_write_chain); // TO DO is it ugly ? II think so
// 		}
// 		catch(const std::exception& e)
// 		{
// 			std::cerr << e.what() << '\n';
// 			// TO DO set error bad request in response status
// 		}
// 		if (end && (_req->getMethod() == "PUT" || _req->getMethod() == "POST") && _stream_write_chain.getFirst() != NULL)
// 			_connection.subStreamWrite();

// 	}
// 	else
// 	{
// 		end = HttpRequest::extractBody(_read_chain, _stream_write_chain, _req);
// 	}
// 	// if end then all body has been received
// 	if (end)
// 	{
// 		// if end reached, then we are done socket side
// 		_status_socket = DONE;
// 		// if stream is none, flush the buffers if they exist TO DO might me optimizable
// 		if (_status_stream_write == NONE)
// 			_stream_write_chain.flush();
// 		// If there's no new buffer, then the stream write is done
// 		else if (_stream_write_chain.size() == 0)
// 			_status_stream_write = DONE;
// 		// unsub read until end of request
// 		_connection.unsubRead();
// 	}
// }