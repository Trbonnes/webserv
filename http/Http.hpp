#if !defined(HTTP)
#define HTTP


#include "core/Connection.hpp"
#include "core/BufferChain.hpp"


#include "HttpResponse.hpp"
// This is the entrypoint of the http module
// All of the filesystem write/read are handled by this module
// The connection socket read and write are handled by the Worker itself

class Http
{

typedef int FD;

private:
	// stream in (CGI or filestream)
	FD _in;

	// stream out
	FD _out;

	// reference to the upstream connection
	Connection& _connection;

	// Headers of the request
	HttpRequest _req;

	// Header of the response
	HttpResponse _rep;

	// Pointers to socket buffer's chain
	BufferChain		_stream_read_chain;
	BufferChain		_stream_write_chain;

	// Reference to connection socket buffer chains
	BufferChain&		_read_chain;
	BufferChain&		_write_chain;

public:
	Http();
	~Http();

};
#endif // HTTP
