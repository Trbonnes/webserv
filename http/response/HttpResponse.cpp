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
    _wwwAuthenticate = "OK";
    _state.read = READY;
    _state.write = WAITING;
    _state.readStream = NONE;
    _state.writeStream = NONE;
    setDate();
}

HttpResponse::HttpResponse(ResponseContext& ctx)
{
    ft_bzero(_date, 100);
    ft_bzero(_lastModified, 100);
    _config = ctx.config;
    _route = ctx.route;
    _location = ctx.location;
    _request = ctx.request;
    _streamWriteFd = -1;
    _streamReadFd = -1;
    _route = ctx.route;
    _statusCode = OK;
    _wwwAuthenticate = "";
    _referer = "";
    _server = "Webserver";
    _contentLength = -1;
    _contentLocation = "";
    _contentType = "";
    _contentLanguage = ctx.language;
    _charset = "";
    _retryAfter = "";
    _transferEncoding = "";
    _wwwAuthenticate = "OK";
    _state.read = READY;
    _state.write = WAITING;
    _state.readStream = NONE;
    _state.writeStream = NONE;
    setDate();
    if (_config)
        _maxBodySize = _config->getClientBodySize(_location);
    if (_request->getContentLength() == -1 && _request->getTransferEncoding() != "chunked")
        _state.read = DONE;
}


void HttpResponse::abort()
{

    // Don't need to close them if they are done
	if (_streamReadFd != -1)
    {
		close(_streamReadFd);
    }
	if (_streamWriteFd != -1)
	{
    	close(_streamWriteFd);
    }
    _streamReadChain.flush();
    _streamWriteChain.flush();
}

void HttpResponse::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
    (void) writeChain;
    // Log::debug("HttpResponse::handleRead()");
    bool	end = false;

	if (_request->getTransferEncoding() == "chunked")
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

    if (end)
    {
        _state.read = DONE;
        if (_streamWriteChain.getFirst() == NULL)
            _state.writeStream = DONE;
    }
    // check if payload too large
    if (_maxBodySize != -1)
    {
        if ((int)_streamWriteChain.totalSize() > _maxBodySize)
            throw HttpError(REQUEST_ENTITY_TOO_LARGE);
    }
    if (_state.writeStream == WAITING && _streamWriteChain.getFirst())
        _state.writeStream = READY;
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
        throw HttpError(INTERNAL_SERVER_ERROR);
    }
    // State update
    if (_state.readStream == DONE || (_state.read == DONE && _streamWriteChain.getFirst() == NULL))
    {
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

static std::string    acceptLanguage(ResponseContext& ctx, std::string& root)
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;


    if (!ctx.config->getLanguage(ctx.location).empty())
    {
        itClientBegin = ctx.request->getAcceptLanguage().begin();
        itClientEnd = ctx.request->getAcceptLanguage().end();
        itServerEnd = ctx.config->getLanguage(ctx.location).end();
        if (itClientBegin == itClientEnd)
            return "";
        // Iterating  through client languages preferences
        while (itClientBegin != itClientEnd)
        {
            
            itServer = std::find(ctx.config->getLanguage(ctx.location).begin(), ctx.config->getLanguage(ctx.location).end(), *itClientBegin);

            // if match
            if (itServer != itServerEnd)
            {
                str.assign(*itServer);
                if (stat(trydir.assign(root).append(str).c_str(), &dir) == 0 && (dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);   
                else
                    str.assign("");
            }
            itClientBegin++;
        }
        // fallback case, try the first one in the server's configuration
        itServer = ctx.config->getLanguage(ctx.location).begin();
        str.append(*itServer);
        if (stat(trydir.assign(root).append(str).c_str(), &dir) == 0 && (dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
    }
    return ("");
}

static void       initRoute(ResponseContext& ctx)
{
    std::vector<std::string>::iterator itIndexBegin;
    std::vector<std::string>::iterator itIndexEnd;
    struct stat file;
    std::string str;
	std::string route = "";
    int ret;

    std::string root = ctx.config->getRoot(ctx.location);  
    std::string alias = ctx.config->getAlias(ctx.location);
    std::string uri = ctx.request->getRequestURI();

    //** Relative path **
    if (alias.length() > 0)
    {
        if (stat(alias.c_str(), &file) != -1)
        {
            if (((file.st_mode & S_IFMT) == S_IFDIR))
                alias.append("/");
            route.assign(alias);
            route.append(uri.substr(ctx.location.size()));
        }
    }
    else 
    {
        route.assign(root.substr(0, root.size() - (root[root.size() - 1] == '*')));
        ctx.language = acceptLanguage(ctx, route);
        if (ctx.language != "")
            route.append(ctx.language).append("/");
        route.append(uri.substr(ctx.location.size() - (ctx.location[ctx.location.size() - 1] == '*' || ctx.location[ctx.location.size() - 1] == '/')));
    }
    ret = stat(route.c_str(), &file);
    // if is file  exists or put request we're done
    if ((ret == 0 && S_ISREG(file.st_mode)) || ctx.request->getMethod() == "PUT" || ctx.request->getMethod() == "DELETE")
    {
        ctx.route = route;
        return ;
    }
    
    size_t pos = route.find_last_of('.');
    std::string extension(route);
    extension.erase(0, pos + 1);
    if (isCgiExtension(ctx.config, ctx.location, extension))
    {
        ctx.route = route;
        std::cout << "Here" << std::endl;
        return ;
    }
    // ** Else, add index if it is not a put or delete request **
    itIndexBegin = ctx.config->getIndex(ctx.location).begin();
    itIndexEnd = ctx.config->getIndex(ctx.location).end();
    // if ile does not exist and cgi incompatible
    if (stat(route.c_str(), &file))
    {
        ctx.route = route;
        return ;
    }
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
    ctx.route = route;
}


// ** Check if the autorization mode is on and if the user is authorized to make the request **
static bool            isAuthorized(ConfigServer* config, HttpRequest* request, std::string& location)
{
    size_t  length;

    if (config->getAuth_basic(location).compare("") != 0)
    {
        if (request->getAuthorization().compare("") == 0)
            return false;
        std::vector<std::string> authorizations = config->getAuthorizations(location);
        std::vector<std::string>::iterator itBegin = authorizations.begin();
        std::vector<std::string>::iterator itEnd = authorizations.end();
        for(; itBegin != itEnd; itBegin++) {
            length = request->getAuthorization().length();
            if (base64_decode(request->getAuthorization().substr(6, length)).compare(*itBegin) == 0)
                return true;
        }
         return false;
    }
    return true;
}


HttpResponse* HttpResponse::newResponse(HttpRequest *request, ConfigServer *config, BufferChain& writeChain)
{
    ResponseContext ctx;

    ctx.request = request;
    // That's the case when the clients send a request that does not match any configuration
    if (config == NULL)
    {
        // we create a new config object so we can get the basic server's headers
        config = new ConfigServer();
        ctx.config = config;
        HttpResponse* err = new Error(ctx, writeChain, BAD_REQUEST);
        delete config;
        return err;
    }
    // Init context
    ctx.config = config;
    ctx.location = config->getLocation(request->getRequestURI());
    initRoute(ctx);

    // Check if length is given
    if (request->getBody().length() > 0 && request->getContentLength() == 0 && request->getTransferEncoding().length() == 0)
        return new Error(ctx, writeChain, LENGTH_REQUIRED);
    // set the route of the ressource
    // Autorization
    if (!isAuthorized(config, request, ctx.location))
        return new Error(ctx, writeChain, UNAUTHORIZED);
    // // If it's a CGI request we must fork and prepare the stream in and out
    size_t pos = ctx.route.find_last_of('.');
    std::string extension(ctx.route);
    extension.erase(0, pos + 1);

    std::string &method = request->getMethod();
    if (isCgiExtension(config, ctx.location, extension) && isMethodAllowedCGI(config, request->getMethod(), ctx.location))
        return  new CgiResponse(ctx, writeChain);
    try
    {
        // If it's not CGI check if the method is authorized
        if (isMethodAllowed(config, method, ctx.location))
        {
            if (method == "GET")
            {
                struct stat file;
                
                if (stat(ctx.route.c_str(), &file))
                    return new Error(ctx, writeChain, NOT_FOUND);
                if (!(file.st_mode & S_IRUSR))
                    return new Error(ctx, writeChain, FORBIDDEN);
                if (S_ISREG(file.st_mode))
                    return new FileDownload(ctx, writeChain, &file);
                if (S_ISDIR(file.st_mode))
                {
                    if (config->getAutoindex(ctx.location) == true)
                            return new FolderIndex(ctx, writeChain, &file);
                    else
                        return new Error(ctx, writeChain, FORBIDDEN);
                }
            }
            if (method == "HEAD")
            {
                struct stat file;
                
                if (stat(ctx.route.c_str(), &file))
                    return new HeadersError(ctx, writeChain, NOT_FOUND);
                if (!(file.st_mode & S_IRUSR))
                    return new HeadersError(ctx, writeChain, FORBIDDEN);
                return new HeadersOnly(ctx, writeChain, &file);
            }
            if (method == "PUT")
                return new FileUpload(ctx);
            if (method == "POST" || method == "OPTIONS")
                return new HeadersOnly(ctx, writeChain, method);
            if (method == "DELETE")
                return new FileDelete(ctx, writeChain);
        }
        // If else the method is not allowed
        else
        {
            if(method == "HEAD")
                return new HeadersError(ctx, writeChain, METHOD_NOT_ALLOWED);
            else
                return new Error(ctx, writeChain, METHOD_NOT_ALLOWED);
        }
    }
    catch(const HttpError& e)
    {
        return new Error(ctx, writeChain, e.getStatusCode());
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
        it = _config->getAllow(_location).begin();
        itEnd = _config->getAllow(_location).end();
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

void			HttpResponse::setStreamReadFd(int fd)
{
    _streamReadFd = fd;
}

void			HttpResponse::setStreamWriteFd(int fd)
{
    _streamWriteFd = fd;
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
