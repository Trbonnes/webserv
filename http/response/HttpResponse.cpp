#include "http/response/HttpResponse.hpp"


HttpResponse::HttpResponse()
{
    _request = NULL;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = route;
    _statusCode = OK;
    _allow = "";
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = 0;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
    _state.write = NONE;
    _state.readStream = NONE;
    _state.writeStream = NONE;
}

HttpResponse::HttpResponse(ConfigServer* config, HttpRequest* req)
{
    _config = config;
    _request = req;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = route;
    _statusCode = OK;
    _allow = "";
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = 0;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
    _state.write = NONE;
    _state.readStream = NONE;
    _state.writeStream = NONE;
}


HttpResponse::HttpResponse(ConfigServer* config, HttpRequest* req, std::string route)
{
    _config = config;
    _route = route;
    _request = req;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = route;
    _statusCode = OK;
    _allow = "";
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = 0;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
    _state.write = NONE;
    _state.readStream = NONE;
    _state.writeStream = NONE;
}


void HttpResponse::abort()
{
	if (_streamReadFd != -1)
		close(_streamReadFd);
	if (_streamWriteFd != -1)
		close(_streamWriteFd);
    _streamReadChain.flush();
    _streamWriteChain.flush();
}

void HttpResponse::handleRead(BufferChain& readChain)
{
    bool	end = false;

	if (_request->getTransferEncoding() == "chunked\r") // TO DO why the f do i have to add \r
	{
		Log::debug("CHUNKING");
		try
		{
			end = HttpRequest::extractChunks(readChain, _streamWriteChain); // TO DO is it ugly ? II think so
		}
		catch(const HttpRequest::MalformedChunk& e)
		{
            throw HttpError(BAD_REQUEST);
		}
	}
	else
	{
		end = HttpRequest::extractBody(readChain, _streamWriteChain, _req);
	}

    // TO DO if payload too large
    // if ()

	// if end then all body has been received
	if (end)
        _state.read = DONE;

    // Stream write state update
	if (_streamWriteChain.getFirst())
        _state.writeStream = READY;
    else if (_state.read == DONE)
        _state.writeStream = DONE;
    else
        _state.writeStream = WAITING;
}

void HttpResponse::handleStreamRead(BufferChain& writeChain)
{
    int ret;
    try
	{
		ret = BufferChain::readToBuffer(writeChain, _streamReadFd);
	}
	catch(const std::exception& e)
	{
		throw HttpError(INTERNAL_SERVER_ERROR);
	}
    // If it's the end
	if (ret == 0)
    {
        // close the stream
        close(_streamReadFd);
        _state.readStream = DONE;
        if (writeChain.getFirst())
            _state.write = READY;
        else
            _state.write = DONE;
    }
}

void HttpResponse::handleStreamWrite()
{
    int ret;

	try
	{
		ret = BufferChain::writeBufferToFd(_streamWriteChain, _streamWriteFd);
		std::string* buff = _streamWriteChain.getFirst();
        // since the fds are non blocking we check if some bytes are left 
		if ((size_t) ret < buff->size())
		{
			std::string * leftovers = new std::string();
			leftovers->reserve(buff->size() - ret);
			leftovers->append(*buff, ret);
			delete buff;
			_streamWriteChain.popFirst();
			_streamWriteChain.pushFront(leftovers);
		}
		else
		{
			delete buff;
			_streamWriteChain.popFirst();
		}
	}
	catch(const BufferChain::IOError& e)
	{
		throw HttpError(INTERNAL_SERVER_ERROR);
	}

    // State update
	if (_streamWriteChain.getFirst() == NULL)
	{
        if (_state.read == DONE)
        {
            close(_streamWriteFd);
            _state.writeStream = DONE;
        }
	}
}

static std::string        getRoute(ConfigServer* config, HttpRequest req, std::string &location)
{
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;
    struct stat file;
    std::string str;
	std::string route;
    int ret;

    std::string root = config->getRoot(location);  
    std::string alias = config->getAlias(location);

    Log::debug(route);
    //** Relative path **
    if (alias.length() > 0)
        route.assign(alias).append("/");
    else
        route.assign(root);
    ret = stat(route.c_str(), &file);
    // if is file  exists or put request we're done
    if ((ret == 0 && S_ISREG(file.st_mode)) || _request->getMethod() == "PUT" || _request->getMethod() == "DELETE")
        return;
    
    // accept lagnguage
    // route.append(acceptLanguage());
    // route.append(str.assign(_request->getRequestURI()).erase(0, location.length()));

    // CGI
    // size_t pos = route.find_last_of('.'); 
    // if (pos != route.npos)
    // {
    //     std::string extension = route.substr(pos + 1);
    //     if (is_good_exe(extension))
    //         return;
    // }
    // ** Else, add index if it is not a put or delete request **
    itIndexBegin = config->getIndex(location).begin();
    itIndexEnd = config->getIndex(location).end();
    stat(route.c_str(), &file);
    str.assign(route);
    while (itIndexBegin != itIndexEnd && !S_ISREG(file.st_mode))
    {
        str.assign(route);
        if (str.at(str.length() - 1) != '/')
            str.append("/");
        str.append(*itIndexBegin);
        stat(str.c_str(), &file);
        itIndexBegin++;
    }
    route.assign(str);
}

static bool       isMethodAllowed(ConfigServer* config, std::string& method, std::string &location)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;

    itBegin = config->getAllow(_location).begin();
    itEnd = config->getAllow(_location).end();
    while (itBegin != itEnd)
    {
        if ((*itBegin) == method)
            return true;
        itBegin++;
    }
    return false;
}

HttpResponse* HttpResponse::newReponse(HttpRequest *request, ConfigServer *config)
{
	std::string uri;
	std::string location;
	std::string route;
    std::string &method = request->getMethod();


	uri = request->getRequestURI();
    location = _config.getLocation(uri);
    
    // Check if length is given
    if (request->getBody().length() > 0 && request->getContentLength() == 0 && request->getTransferEncoding().length() == 0)
        return new Error(LENGTH_REQUIRED);
    
    // set the route of the ressource
    route = getRoute();

    // TO DO insert authorization mechanic


    // size_t      extension;
    // std::string str;
    // extension = route.find_last_of('.');
    // If it's a CGI request we must fork and prepare the stream in and out
    // if (is_good_exe(str.assign(route).erase(0, extension + 1)) && checkCGImethods(request->getMethod()))
    // {
    //     cgi();
    // }

    // If it's not CGI check if the method is authorized
    if (isMethodAllowed(config, method, location)
    {
        if (method == "GET")
        {
            struct stat file; 
            
            if (stat(_route.c_str(), &file))
                return new Error(NOT_FOUND);

            if (S_IFREG(file.st_mode))
                return new FileDownload(route);

            if (S_ISDIR(file.st_mode))
            {
                if (config->getAutoindex(location) == true)
                    return new FolderIndex(route);
                else
                    return new Error(FORBIDDEN);
            }
        }
    }
    // If else the method is not allowed
    else
        return new Error(METHOD_NOT_ALLOWED);

}

std::string HttpResponse::*getRawHeaders()
{

}
