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
		if (ft_strnstr(_read_chain.getFirst().data, "\r\n\r\n", _read_chain.getFirst().size))
		{
			std::cout << "End of header" << std::endl;
		}
	}
		_req = new HttpRequest();
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
