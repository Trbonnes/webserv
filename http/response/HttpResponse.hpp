#if !defined(HTTP_RESPONSE)
#define HTTP_RESPONSE


#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

// Core includes
#include "core/BufferChain.hpp"
#include "core/ConfigServer.hpp"

// Http includes
#include "http/HttpRequest.hpp"

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
	HttpResponse(ConfigServer*, HttpRequest*);
	HttpResponse(ConfigServer*, HttpRequest*, std::string route);
	virtual ~HttpResponse();

	//public funciton
	std::string *getRawHeaders();
	
	// public virtual fucntions
	// abort the current request
	virtual void abort();

	// Read/write eent processing, might be redefined by child classes
	virtual void handleRead(BufferChain& readChain);
	virtual void handleWrite(BufferChain& writeChain);
	virtual void handleStreamRead(BufferChain& writeChain); 
	virtual void handleStreamWrite();


	//Getters
	BufferChain&	getStreamReadChain();
	BufferChain&	getStreamWriteChain();
	FD				getStreamReadFd();
	FD				getStreamWriteFd();

	//Setters
	void        	setServerName();
	void        	setContentLocation();
	void        	setDate();
	void  	      	setCharset(void);
	void    	    setContentType();
	void        	setLastModified(struct stat* file);


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

#include "http/response/Error.hpp"
#include "http/response/FileDownload.hpp"
#include "http/response/FolderIndex.hpp"
#include "http/response/HeadersOnly.hpp"

#endif // HTTP_RESPONSE
