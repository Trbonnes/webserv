#include "Http.hpp"

Http::Http(Connection &c) :
_connection(c),
_read_chain(c.getReadChain()),
_write_chain(c.getWriteChain())
{
	_req = NULL;
	_rep = NULL;
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
	// first time reading a a freshly accepted connection
	if (_req == NULL)
	{
		char* needle;
		BufferChain::buffer_t* last = _read_chain.getLast();
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
			std::cout << request << std::endl;
			std::cout << "====================================" << std::endl;
			if(_stream_write_chain.getFirst())
			{
				std::cout << "oooooooooooooooooooooooooooooooooooo" << std::endl;	
				std::cout << "|" << std::string(_stream_write_chain.getFirst()->data, _stream_write_chain.getFirst()->size)  << "|" << std::endl;
				std::cout << "oooooooooooooooooooooooooooooooooooo" << std::endl;
			}
			// _req = new HttpRequest();
		}
	}
	// if null then headers are not fully received
	if (_rep == NULL)
	{
		//

	}
	else
	{
		// Process headers
	}
}

// void Http::handleStreamRead()
// {

// }

Http::~Http()
{
	if (_req)
		delete _req;
	if (_rep)
		delete _rep;
}
