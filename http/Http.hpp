#if !defined(HTTP)
#define HTTP


#include <string>
#include <sstream> // TO DO is this legal ?

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
	HttpResponseOld *_resp;

	// Pointers to socket buffer's chain
	BufferChain		_stream_read_chain;
	BufferChain		_stream_write_chain;

	// Reference to connection socket buffer chains
	BufferChain&	_read_chain;
	BufferChain&	_write_chain;
	
	// the servers configuration
	Config* _config;

	std::string		_requestBuffer;
	std::string		_streamBuffer;
	// Prevent call to these functions
	Http();

	// TO OD remove those fuckers
	size_t _bodySend;
	size_t _bodyStreamWritten;
	size_t _bodyRec;

	// check the sate of the module and resets it if needed
	void	checkState();
	// reset the state of the module
	void	reset();

	// Utils function to copy a chunk into a new buffer
	int				readChunkToBuffer(BufferChain&, FD);
	std::string*	chunkify(char*, size_t, size_t);


	typedef enum e_status
	{
		NONE,
		WAITING_HTTP_HEADERS,
		WAITING_HTTP_BODY,
		WAITING_CGI_HEADERS,
		WAITING_STREAM_READ,
		WAITING_STREAM_WRITE,
		DONE
	} status_t;

	status_t _status_socket;
	status_t _status_stream_read;
	status_t _status_stream_write;

public:
	Http(Connection &c);
	Http(const Http&);
	Http& operator=(const Http&);
	~Http();

	// Set the module's configuration
	void setConfig(Config*);

	void handleRead();
	void handleWrite();
	void handleStreamWrite();
	void handleStreamRead();

	void handleNewRequest();
	void handleBodyRead();


	void handleCGIRead();


};

#include "core/Connection.hpp"

#endif // HTTP
