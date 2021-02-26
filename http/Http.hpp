#if !defined(HTTP)
#define HTTP


#include <string>

#include "core/BufferChain.hpp"
#include "core/Config.hpp"
#include "HttpResponse.hpp"
// This is the entrypoint of the http module
// All of the filesystem write/read are handled by this module
// The connection socket read and write are handled by the Worker itself
class Connection;

class Http
{

typedef int FD;

private:
	// reference to the upstream connection
	Connection& _connection;

	// Headers of the request
	HttpRequest *_req;

	// Header of the response
	HttpResponse *_resp;

	// Pointers to socket buffer's chain
	BufferChain		_stream_read_chain;
	BufferChain		_stream_write_chain;

	// Reference to connection socket buffer chains
	BufferChain&	_read_chain;
	BufferChain&	_write_chain;
	
	// the servers configuration
	Config* _config;

	std::string		_requestBuffer;
	// Prevent call to these functions
	Http();


	typedef enum e_status
	{
		WAITING_HEADERS,
		WAITING_BODY,
		// WAITING_STREAM_WRITE,
		// WAITING_STREAM_READ,
		// INACTIVE,
		DONE
	} status_t;

	status_t _status_socket;
	status_t _status_stream;
public:
	Http(Connection &c);
	Http(const Http&);
	Http& operator=(const Http&);
	~Http();

	// Set the module's configuration
	void setConfig(Config*);

	void handleRead();
	void handleStreamWrite();
	void handleStreamRead();

	void handleNewRequest();
	void handleBodyRead();

	// destroy the HttpRequest object
	void	destroyRequest();
	void	destroyResponse();

};

#include "core/Connection.hpp"

#endif // HTTP
