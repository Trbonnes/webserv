#if !defined(HTTP_RESPONSE)
#define HTTP_RESPONSE


#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <algorithm>

// Core includes
#include "core/BufferChain.hpp"
#include "core/ConfigServer.hpp"

// Http includes
#include "http/HttpRequest.hpp"



typedef struct s_responseContext
{
	ConfigServer* config;
	HttpRequest* request;
	std::string location;
	std::string route;
	std::string language;
} ResponseContext;


class HttpResponse
{
	
	protected:

	// Pointer to the request
    HttpRequest*                _request;
    ConfigServer*               _config;

	//Utils
	std::string 				_location;

	// used to store errors string
	StatusCode					_mapCodes;

    // File streams
    FD                         _streamWriteFd;
    FD                         _streamReadFd;
	// Max body size allowed to read, -1 if no limits
	int							_maxBodySize;

	// BufferChain
	BufferChain					_streamWriteChain;
	BufferChain					_streamReadChain;

	// Utils
	std::string                 _route;

	// headers
	int                         _statusCode;
	std::vector<std::string>    _allow;
	std::string                 _wwwAuthenticate;
	std::string                 _referer;
	char                        _lastModified[100];
	char                        _date[100];
	std::string                 _server;
	std::string                 _contentLanguage;
	int                         _contentLength;
	std::string                 _contentLocation;
	std::string                 _contentType;
	std::string                 _charset;
	std::string                 _retryAfter;
	std::string                 _transferEncoding;


	std::string					acceptLanguage();

public:

	// Public types

	typedef enum	e_status
	{
		NONE,
		WAITING,
		IGNORE,
		READY,
		DONE
	}				t_status;

	// Strctire to monitor the state of the response
	typedef struct	s_state
	{
		t_status read;
		t_status write;
		t_status readStream;
		t_status writeStream;
	}				t_state;

	// Public attributes members
	t_state _state;

	//Coplien // TODO
	HttpResponse();
	HttpResponse(ResponseContext&);
	virtual ~HttpResponse();

	//public funciton
	virtual std::string *getRawHeaders();
	
	// public virtual fucntions
	// abort the current request
	virtual void abort();

	// Read/write eent processing, might be redefined by child classes
	virtual void handleRead(BufferChain& readChain, BufferChain& writeChain);
	virtual void handleWrite(BufferChain& readChain, BufferChain& writeChain);
	virtual void handleStreamRead(BufferChain& readChain, BufferChain& writeChain); 
	virtual void handleStreamWrite(BufferChain& readChain, BufferChain& writeChain);


	//Getters
	BufferChain&	getStreamReadChain();
	BufferChain&	getStreamWriteChain();
	FD				getStreamReadFd();
	FD				getStreamWriteFd();
	std::string&	getRoute();
	std::string&	getLocation();
	ConfigServer*	getConfig();

	//Setters
	void        	setServerName();
	void        	setContentLocation();
	void        	setDate();
	void  	      	setCharset(void);
	void    	    setContentType();
	void        	setLastModified(struct stat* file);
	void			setStreamReadFd(int fd);
	void			setStreamWriteFd(int fd);



	// Returns a newResponse based on the Request and Conffiguration
	static HttpResponse* newResponse(HttpRequest *socket, ConfigServer *config, BufferChain& writeChain);

	class   HttpError : public std::exception
	{
		int _statusCode;

		public:
			HttpError(int code) : _statusCode(code) {}
			const char * what () const throw () {return "HttpError ecountered";}
			int getStatusCode() const {return _statusCode;}
	};

	class   ConnectionClose : public std::exception
	{
		public:
			const char * what () const throw () {return "Closing the connection";}
	};
};

std::string   base64_decode(std::string const& encoded_string);

#include "http/response/HeadersOnly.hpp"
#include "http/response/Error.hpp"
#include "http/response/FileDownload.hpp"
#include "http/response/FolderIndex.hpp"
#include "http/response/FileUpload.hpp"
#include "http/response/CgiResponse.hpp"

#endif // HTTP_RESPONSE
