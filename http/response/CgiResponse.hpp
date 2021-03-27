#if !defined(CGIRESPONSE_HPP)
#define CGIRESPONSE_HPP

#include "HttpResponse.hpp"
#include "http/Http.hpp"

#include <signal.h>
#include <string>

# define SIDE_OUT 0
# define SIDE_IN 1

class CgiResponse : public HttpResponse
{
private:

	enum cgi_variables
	{
		REDIRECT_STATUS,
		AUTH_TYPE,
		CONTENT_TYPE,
		GATEWAY_INTERFACE,
		PATH_INFO,
		PATH_TRANSLATED,
		QUERY_STRING,
		REMOTE_ADDR,
		REMOTE_IDENT,
		REMOTE_USER,
		REQUEST_METHOD,
		REQUEST_URI,
		SCRIPT_NAME,
		SERVER_NAME,
		SERVER_PORT,
		SERVER_PROTOCOL,
		SERVER_SOFTWARE,
		X_SECRET,
		CONTENT_LENGTH,
		NB_METAVARIABLES
	};

	struct              CGI
	{
		std::string     _redirect_status;
		std::string     _auth_type;
		std::string     _content_type;
		std::string     _gateway_interface;
		std::string     _path_info;
		std::string     _path_translated;
		std::string     _query_string;
		std::string     _remote_addr;
		std::string     _remote_ident;
		std::string     _remote_user;
		std::string     _request_method;
		std::string     _request_uri;
		std::string     _script_name;
		std::string     _server_name;
		std::string     _server_port;
		std::string     _server_protocol;
		std::string     _server_software;
		std::string     _content_length;

	};

	pid_t 		_cgipid;
	bool		_headersReceived;
	struct CGI	_cgi;
	char*		_cgi_env[NB_METAVARIABLES + 1];
	std::string	_streamBuffer;

	void        cgi_metaVariables();
	void        setEnv();

public:
	CgiResponse(ConfigServer* config, HttpRequest* request,std::string route, std::string location,  BufferChain& writeChain);
	std::string* getRawHeaders();
	~CgiResponse();

	void abort();
	void handleStreamRead(BufferChain& readChain, BufferChain& writeChain);
};

#endif // HEADERSONLY_HPP
