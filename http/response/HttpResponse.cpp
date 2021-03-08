#include "HttpResponse.hpp"


HttpResponse::HttpResponse()
{
    ft_bzero(_date, 100);
    _request = NULL;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = "";
    _statusCode = OK;
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = -1;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
    _state.readStream = NONE;
    _state.writeStream = NONE;
}

HttpResponse::HttpResponse(ConfigServer* config, HttpRequest* req)
{
    ft_bzero(_date, 100);
    _config = config;
    _request = req;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = "";
    _statusCode = OK;
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = -1;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
    _state.readStream = NONE;
    _state.writeStream = NONE;
}


HttpResponse::HttpResponse(ConfigServer* config, HttpRequest* req, std::string route)
{
    ft_bzero(_date, 100);
    _config = config;
    _route = route;
    _request = req;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = route;
    _statusCode = OK;
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLanguage = "";
    _contentLength = -1;
    _contentLocation = "";
    _contentType = "";
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _state.read = NONE;
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
		end = HttpRequest::extractBody(readChain, _streamWriteChain, _request);
	}

    // TO DO if payload too large
    // if ()

	// if end then all body has been received
	if (end)
        _state.read = DONE;

    // Stream write state update
	if (_state.writeStream != NONE)
    {
        if (_streamWriteChain.getFirst())
            _state.writeStream = READY;
        else if (_state.read == DONE)
            _state.writeStream = DONE;
        else
            _state.writeStream = WAITING;
    }
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

static std::string        getRoute(ConfigServer* config, HttpRequest* req, std::string &location)
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
    if ((ret == 0 && S_ISREG(file.st_mode)) || req->getMethod() == "PUT" || req->getMethod() == "DELETE")
        return route;
    
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
    return route;
}

static bool       isMethodAllowed(ConfigServer* config, std::string& method, std::string &location)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;

    itBegin = config->getAllow(location).begin();
    itEnd = config->getAllow(location).end();
    while (itBegin != itEnd)
    {
        if ((*itBegin) == method)
            return true;
        itBegin++;
    }
    return false;
}

HttpResponse* HttpResponse::newResponse(HttpRequest *request, ConfigServer *config, BufferChain& writeChain)
{
	std::string uri;
	std::string location;
	std::string route;
    std::string &method = request->getMethod();


	uri = request->getRequestURI();
    location = config->getLocation(uri);
    
    // Check if length is given
    if (request->getBody().length() > 0 && request->getContentLength() == 0 && request->getTransferEncoding().length() == 0)
        return new Error(config, request, writeChain, LENGTH_REQUIRED);
    
    // set the route of the ressource
    route = getRoute(config, request, location);

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
    if (isMethodAllowed(config, method, location))
    {
        if (method == "GET")
        {
            struct stat file; 
            
            if (stat(route.c_str(), &file))
                return new Error(config, request, writeChain, NOT_FOUND);
            if (!(file.st_mode & S_IRUSR))
                return new Error(config, request, writeChain, FORBIDDEN);
            if (S_ISREG(file.st_mode))
                return new FileDownload(config, request, route, location, &file);
            if (S_ISDIR(file.st_mode))
            {
                if (config->getAutoindex(location) == true)
                        return new FolderIndex(config, request, route, writeChain);
                else
                    return new Error(config, request, writeChain, FORBIDDEN);
            }
        }
    }
    // If else the method is not allowed
    else
        return new Error(config, request, writeChain, METHOD_NOT_ALLOWED);

    return NULL;
}

std::string* HttpResponse::getRawHeaders()
{
    std::string* buff = new std::string();

    buff->append(_config->getHttpVersion());
    buff->append(" ");
    char *tmp = ft_itoa(_statusCode);
    buff->append(tmp).append(" ");
    free(tmp);
    buff->append(_mapCodes.codes[_statusCode]).append("\r\n");
    buff->append("Server: ").append(_config->getServerSoftware()).append("\r\n");
    if (ft_strlen(_date) > 0)
        buff->append("Date: ").append(_date).append("\r\n");
    if (_contentType.length() > 0)
        buff->append("Content-Type: ").append(_contentType).append("\r\n");
    if (_contentLength >= 0)
    {
        tmp = ft_itoa(_contentLength);
        buff->append("Content-Length: ").append(tmp).append("\r\n");
        free(tmp);
    }

    if (_request->getMethod().compare("OPTIONS") == 0 || _statusCode == METHOD_NOT_ALLOWED)
    {
        std::vector<std::string>::iterator it;
        std::vector<std::string>::iterator itEnd;
        std::size_t extension;
        std::string str;

        buff->append("Allow: ");
        extension = _route.find_last_of('.');
        // if (is_good_exe(str.assign(_route).erase(0, extension + 1)))
        // {
        //     it = _config->getCGI_allow(_location).begin();
        //     itEnd = _config->getCGI_allow(_location).end();
        // }
        // else
        // {
            it = _config->getAllow(_location).begin();
            itEnd = _config->getAllow(_location).end();
        // }
        while (it != itEnd)
        {
            buff->append(*it).append(" ");
            it++;
        }
        buff->append("\r\n");
    }
    else
    {
        if (_charset.length() > 0)
            buff->append("Charset: ").append(_charset).append("\r\n");
        if (_statusCode < 300)
        {
            if (ft_strlen(_lastModified) > 0)
                buff->append("Last-Modified: ").append(_lastModified).append("\r\n");
            if (_contentLocation.length() > 0)
                buff->append("Content-Location: ").append(_contentLocation).append("\r\n");
            if (_contentLanguage.length() > 0 && _request->getMethod().compare("PUT") && _request->getMethod().compare("DELETE"))
                buff->append("Content-Language: ").append(_contentLanguage).append("\r\n");
            if (_transferEncoding.length() > 0)
                buff->append("Transfer-Encoding: ").append(_transferEncoding);
        }
        else if (_statusCode == UNAUTHORIZED)
            buff->append("WWW-Authenticate: ").append("Basic realm=").append(_config->getAuth_basic(_location)).append("\r\n");
    }
    buff->append("\r\n");
    return buff;
}

BufferChain&	HttpResponse::getStreamReadChain()
{
    return _streamReadChain;
}

BufferChain&	HttpResponse::getStreamWriteChain()
{
    return _streamWriteChain;
}

FD				HttpResponse::getStreamReadFd()
{
    return _streamReadFd;
}

FD				HttpResponse::getStreamWriteFd()
{
    return _streamWriteFd;
}

void        HttpResponse::setServerName()
{
    _server = _request->getHost();
}


void        HttpResponse::setContentLocation()
{
    _contentLocation.assign("http://").append(_request->getHost()).append("/").append(_route);
}

void        HttpResponse::setDate()
{
    struct timeval  tv;
    struct tm       *timeinfo;

    if (gettimeofday(&tv, NULL) == 0)
    {
        timeinfo = localtime(&(tv.tv_sec));
        strftime(_date, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
    }
}

HttpResponse::~HttpResponse()
{
    
}