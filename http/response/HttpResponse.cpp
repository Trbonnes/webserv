#include "HttpResponse.hpp"


HttpResponse::HttpResponse()
{
    ft_bzero(_date, 100);
    ft_bzero(_lastModified, 100);
    _streamWriteFd = -1;
    _streamReadFd = -1;
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
    _state.read = READY;
    _state.write = WAITING;
    _state.readStream = NONE;
    _state.writeStream = NONE;
    setDate();
}

HttpResponse::HttpResponse(ConfigServer* config, HttpRequest* req, std::string route, std::string& location)
{
    ft_bzero(_date, 100);
    ft_bzero(_lastModified, 100);
    _config = config;
    _route = route;
    _location = location;
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
    _state.read = READY;
    _state.write = WAITING;
    _state.readStream = NONE;
    _state.writeStream = NONE;
    setDate();
    _maxBodySize = _config->getClientBodySize(_location);
    if (req->getContentLength() == -1 && req->getTransferEncoding() != "chunked\r")
    {
        _state.read = DONE;
    }
}


void HttpResponse::abort()
{

    // Don't need to close them if they are done
	if (_state.readStream != NONE && _state.readStream != DONE)
		close(_streamReadFd);
	if (_state.writeStream != NONE && _state.writeStream != DONE)
		close(_streamWriteFd);
    _streamReadChain.flush();
    _streamWriteChain.flush();
}

void HttpResponse::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
    (void) writeChain;
    // Log::debug("HttpResponse::handleRead()");
    bool	end = false;

	if (_request->getTransferEncoding() == "chunked\r") // TO DO why the f do i have to add \r
	{
		try
		{
			end = HttpRequest::extractChunks(readChain, _streamWriteChain);
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

    // check if payload too large
    if (_maxBodySize != -1)
    {
        if ((int)_streamWriteChain.totalSize() > _maxBodySize)
            throw HttpError(REQUEST_ENTITY_TOO_LARGE);
    }
    if (end)
    {
        _state.read = DONE;
        if (_streamWriteChain.getFirst() == NULL)
        {
            close(_streamWriteFd);
            _state.writeStream = DONE;
        }
    }
    if (_state.writeStream == WAITING && _streamWriteChain.getFirst())
    {
        _state.writeStream = READY;
    }
}

void HttpResponse::handleWrite(BufferChain& readChain, BufferChain& writeChain)
{
    // Log::debug("HttpResponse::handleWrite()");
    (void) writeChain;
    (void) readChain;
}

void HttpResponse::handleStreamRead(BufferChain& readChain, BufferChain& writeChain)
{
    // Log::debug("HttpResponse::handleStreamRead()");
    (void) readChain;
    (void) writeChain;

    int ret;
    try
	{
        if (_transferEncoding == "chunked")
            ret = Http::readChunkToBuffer(writeChain, _streamReadFd);
        else
		    ret = BufferChain::readToBuffer(writeChain, _streamReadFd);
	}
	catch(const std::exception& e)
	{
		throw HttpError(INTERNAL_SERVER_ERROR);
	}
	if (ret == 0)
    // If it's the end
    {
        
        // std::cout << "Closing " << _streamReadFd << std::endl;
        // close the stream
        close(_streamReadFd);
        _state.readStream = DONE;
    }
}

void HttpResponse::handleStreamWrite(BufferChain& readChain, BufferChain& writeChain)
{
    // Log::debug("HttpResponse::handleStreamWrite()");
    (void) readChain;
    (void) writeChain;
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
        std::cout << getpid() << "HANDLE STREAM WRIIIIIIIIIIIIITE  " << strerror(errno)  << " " << _streamWriteFd << std::endl;
        throw HttpError(INTERNAL_SERVER_ERROR);
    }
    // State update
    if (_state.readStream == DONE || (_state.read == DONE && _streamWriteChain.getFirst() == NULL))
    {
        // std::cout << "Closing in stream write " << _streamWriteFd << std::endl;
        close(_streamWriteFd);
        _state.writeStream = DONE;
    }
    else if (_streamWriteChain.getFirst() == NULL)
        _state.writeStream = WAITING;
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

//** Check if the method is autorized for the CGI locations **
static bool         isMethodAllowedCGI(ConfigServer* config, std::string& method, std::string& location)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;

    itBegin = config->getCGI_allow(location).begin();
    itEnd = config->getCGI_allow(location).end();
    while (itBegin != itEnd)
    {
        if ((*itBegin).compare(method) == 0)
            return true;
        itBegin++;
    }
    return false;
}

static bool         isCgiExtension(ConfigServer* config, std::string& location, std::string& exe)
{
    std::vector<std::string>::iterator cgi_begin;
    std::vector<std::string>::iterator cgi_end;

    if (config->getCGI_root(location).length() > 0)
    {
        if (config->getCGI(location).empty() == false)
        {
            cgi_begin = config->getCGI(location).begin();
            cgi_end = config->getCGI(location).end();
            while (cgi_begin != cgi_end)
            {
                if (exe.compare(*cgi_begin) == 0)
                    return true;
                cgi_begin++;
            }
        }
    }
    return false;
}

static std::string        createRoute(ConfigServer* config, HttpRequest* req, std::string location)
{
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;
    struct stat file;
    std::string str;
	std::string route = "";
    int ret;

    std::string root = config->getRoot(location);  
    std::string alias = config->getAlias(location);
    std::string uri = req->getRequestURI();


    //** Relative path **
    if (alias.length() > 0)
    {
        if (stat(alias.c_str(), &file) != -1)
        {
            if (((file.st_mode & S_IFMT) == S_IFDIR))
                alias.append("/");
            route.assign(alias);
            route.append(uri.substr(location.size()));
        }
    }
    else 
    {
        route.assign(root.substr(0, root.size() - (root[root.size() - 1] == '*')));
        route.append(uri.substr(location.size() - (location[location.size() - 1] == '*')));
    }
    // std::cout << "=========== root:" << root << std::endl << "=========== alias:" << alias << std::endl << "=========== uri:" << uri << std::endl << "=========== location:" << location << std::endl;
    ret = stat(route.c_str(), &file);
    // if is file  exists or put request we're done
    if ((ret == 0 && S_ISREG(file.st_mode)) || req->getMethod() == "PUT" || req->getMethod() == "DELETE")
        return route;
    

    // TO DO redundant
    size_t pos = route.find_last_of('.');
    std::string extension(route);
    extension.erase(0, pos + 1);
    if (isCgiExtension(config, location, extension))
        return route;

    // accept lagnguage
    // route.append(acceptLanguage());
    // route.append(str.assign(_request->getRequestURI()).erase(0, location.length())); // TO DO how does this works


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
        return new Error(config, request, route, location, writeChain, LENGTH_REQUIRED);
    
    // set the route of the ressource
    route = createRoute(config, request, location);

    // TO DO insert authorization mechanic


    // // If it's a CGI request we must fork and prepare the stream in and out
    size_t pos = route.find_last_of('.');
    std::string extension(route);
    extension.erase(0, pos + 1);
    if (isCgiExtension(config, location, extension) && isMethodAllowedCGI(config, request->getMethod(), location))
        return  new CgiResponse(config, request, route, location);

    // If it's not CGI check if the method is authorized
    if (isMethodAllowed(config, method, location))
    {
        if (method == "GET")
        {
            struct stat file;
            
            if (stat(route.c_str(), &file))
                return new Error(config, request, route, location, writeChain, NOT_FOUND);
            if (!(file.st_mode & S_IRUSR))
                return new Error(config, request, route, location, writeChain, FORBIDDEN);
            if (S_ISREG(file.st_mode))
                return new FileDownload(config, request, route, location, writeChain, &file);
            if (S_ISDIR(file.st_mode))
            {
                if (config->getAutoindex(location) == true)
                        return new FolderIndex(config, request, route, location, writeChain, &file);
                else
                    return new Error(config, request, route, location, writeChain, FORBIDDEN);
            }
        }
        if (method == "HEAD")
        {
            struct stat file;
            
            if (stat(route.c_str(), &file))
                return new HeadersError(config, request, route, location, writeChain, NOT_FOUND);
            if (!(file.st_mode & S_IRUSR))
                return new HeadersError(config, request, route, location, writeChain, FORBIDDEN);
            return new HeadersOnly(config, request, route, location, writeChain, &file);
        }
        if (method == "PUT")
        {
            return new FileUpload(config, request, route, location);
        }
        if (method == "POST")
        {
            return new HeadersOnly(config, request, route, location, writeChain);
        }
    }
    // If else the method is not allowed
    else
    {
        if(method == "HEAD")
            return new HeadersError(config, request, route, location, writeChain, METHOD_NOT_ALLOWED);
        else
            return new Error(config, request, route, location, writeChain, METHOD_NOT_ALLOWED);
    }

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
                buff->append("Transfer-Encoding: ").append(_transferEncoding).append("\r\n");
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

std::string&	HttpResponse::getRoute()
{
    return _route;
}

ConfigServer*	HttpResponse::getConfig()
{
    return _config;
}

std::string&	HttpResponse::getLocation()
{
    return _location;
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

void        HttpResponse::setCharset(void)
{
    _charset = _config->getCharset(_location);
}

void        HttpResponse::setLastModified(struct stat* file)
{
    struct tm *timeinfo;

	#ifdef __linux__
    		timeinfo = localtime(&(file->st_mtim.tv_sec));
	#else
            timeinfo = localtime(&(file->st_mtimespec.tv_sec)); // st_mtimespec.tv_sec = macos ; st_mtim = linux
	#endif // TARGET_OS_MAC
    strftime(_lastModified, 100, "%a %d %b 20%y %OH:%OM:%OS GMT", timeinfo);
}


void        HttpResponse::setContentType()
{
    int                     find;
    int                     length;
    std::map<std::string, std::string>::iterator it;

    find = _route.find_last_of('.');
    find += 1;
    length = _route.length() - find;
    _contentType = _route.substr(find, length);
    find = -1;
    it = _config->getMimeTypes().begin();

    while (it != _config->getMimeTypes().end())
    {
        if ((it->first).compare(_contentType) == 0)
            break;
        it++;
    }
    if (it != _config->getMimeTypes().end())
        _contentType = it->second;
    else
        _contentType = _config->getType(_location);
}

HttpResponse::~HttpResponse()
{
    abort();
}
