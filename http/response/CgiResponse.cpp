#include "CgiResponse.hpp"


CgiResponse::CgiResponse(ConfigServer* config, HttpRequest* request,std::string route, std::string location, BufferChain& writeChain) : HttpResponse(config, request, route, location)
{
	_headersReceived = false;
	_cgipid = -1;
	_location = location;
    _transferEncoding.assign("chunked");
	cgi_metaVariables();
	setEnv();

    char*       args[2];
    int         cgi_in[2];
    int         cgi_out[2];


    // If it's a head request or an options, just don't bother with all the rest
    if (request->getMethod() == "HEAD" || request->getMethod() == "OPTIONS")
    {
        writeChain.pushBack(getRawHeaders());
        _state.write = READY;
        return;
    }

    // first pipe
    if (pipe(cgi_in))
    {
		// TO DO throw error
        Log::debug("PIPE 1 FAILLLLLLLLLLLED");
        return;
    }

    // second pipe
    if (pipe(cgi_out))
    {
        Log::debug("PIPE 2 FAILLLLLLLLLLLED");
        close(cgi_in[0]);
        close(cgi_in[1]);
		// TO DO throw error
        return;
    }
    // Fork
    _cgipid = fork();
    if (_cgipid < 0)
    {
        Log::debug("FORK FAILLLLLLLLLLLED");
        _statusCode = INTERNAL_SERVER_ERROR;
    }		// TO DO throw error
    else if (_cgipid == 0)
    {
        // Log::debug("FORK GOOD");

        // Closing unused end in chlid process
        close(cgi_in[SIDE_IN]);
        close(cgi_out[SIDE_OUT]);
        // Redirecting the other pipe to stdin and stdout
        dup2(cgi_in[SIDE_OUT], STDIN_FILENO);
        dup2(cgi_out[SIDE_IN], STDOUT_FILENO);

        args[0] = ft_strdup(_config->getCGI_root(_location).c_str());
		args[1] = NULL;
        if (execve(args[0], args, _cgi_env) == -1)
        {
            Log::debug("CGI launch error");
            exit(EXIT_FAILURE);
        }
        // TO DO test for bad gateway just in case
    }
    else
    {
        // closing unused pipe in parent and storing fd fd in streams
        close(cgi_in[SIDE_OUT]);
        close(cgi_out[SIDE_IN]);
        _streamWriteFd = cgi_in[SIDE_IN];
        _streamReadFd = cgi_out[SIDE_OUT];
        fcntl(_streamWriteFd, F_SETFL, O_NONBLOCK);
        fcntl(_streamReadFd, F_SETFL, O_NONBLOCK);
		

        // STAte update
        if (_state.read == DONE)
        {
            close(_streamWriteFd);
            _state.writeStream = DONE;
        }
        else
        {
            if (_streamWriteChain.getFirst())
                _state.writeStream = READY;
            else
                _state.writeStream = WAITING;
        }
        _state.readStream = READY;
    }
}


// ** Define the meta variables for the CGI script **
void        CgiResponse::cgi_metaVariables()
{
    std::string str;
    size_t      query;

    _cgi._redirect_status = "200";
    _cgi._auth_type = _request->getAuthorization();
	char * tmp = ft_itoa(_request->getContentLength());
    _cgi._content_length.assign(tmp);
	free(tmp);
    _cgi._content_type = _request->getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    query = str.assign(_route).find('?', 0);
    _cgi._path_info = _request->getRequestURI();
    _cgi._path_translated = _route;
    if (query == str.npos)
       _cgi._query_string = str.assign(_route).erase(0, query);
    _cgi._remote_addr = "127.0.0.1";
    _cgi._remote_ident = "login_user";
    _cgi._remote_user = "user";
    _cgi._request_method = _request->getMethod();
    _cgi._request_uri = _request->getRequestURI();
    _cgi._script_name = _config->getCGI_root(_location);
    _cgi._server_name = _request->getHost();
    tmp = ft_itoa(_request->getPort());
    _cgi._server_port = tmp;
    free(tmp);
    _cgi._server_protocol = _config->getHttpVersion();
    _cgi._server_software = _config->getServerSoftware();
    return ;
}

// ** Set the environnement variables in a char** table **
void        CgiResponse::setEnv()
{
    _cgi_env[REDIRECT_STATUS] = ft_strdup(_cgi._redirect_status.insert(0, "REDIRECT_STATUS=").c_str());
    _cgi_env[AUTH_TYPE] = ft_strdup(_cgi._auth_type.insert(0, "AUTH_TYPE=").c_str());
    _cgi_env[CONTENT_TYPE] = ft_strdup(_cgi._content_type.insert(0, "CONTENT_TYPE=").c_str());
    _cgi_env[GATEWAY_INTERFACE] = ft_strdup(_cgi._gateway_interface.insert(0, "GATEWAY_INTERFACE=").c_str());
    _cgi_env[PATH_INFO] = ft_strdup(_cgi._path_info.insert(0, "PATH_INFO=").c_str());
    _cgi_env[PATH_TRANSLATED] = ft_strdup(_cgi._path_translated.insert(0, "PATH_TRANSLATED=").c_str());
    _cgi_env[QUERY_STRING] = ft_strdup(_cgi._query_string.insert(0, "QUERY_STRING=").c_str());
    _cgi_env[REMOTE_ADDR] = ft_strdup(_cgi._remote_addr.insert(0, "REMOTE_ADDR=").c_str());
    _cgi_env[REMOTE_IDENT] = ft_strdup(_cgi._remote_ident.insert(0, "REMOTE_IDENT=").c_str());
    _cgi_env[REMOTE_USER] = ft_strdup(_cgi._remote_user.insert(0, "REMOTE_USER=").c_str());
    _cgi_env[REQUEST_METHOD] = ft_strdup(_cgi._request_method.insert(0, "REQUEST_METHOD=").c_str());
    _cgi_env[REQUEST_URI] = ft_strdup(_cgi._request_uri.insert(0, "REQUEST_URI=").c_str());
    _cgi_env[SCRIPT_NAME] = ft_strdup(_cgi._script_name.insert(0, "SCRIPT_NAME=").c_str());
    _cgi_env[SERVER_NAME] = ft_strdup(_cgi._server_name.insert(0, "SERVER_NAME=").c_str());
    _cgi_env[SERVER_PORT] = ft_strdup(_cgi._server_port.insert(0, "SERVER_PORT=").c_str());
    _cgi_env[SERVER_PROTOCOL] = ft_strdup(_cgi._server_protocol.insert(0, "SERVER_PROTOCOL=").c_str());
    _cgi_env[SERVER_SOFTWARE] = ft_strdup(_cgi._server_software.insert(0, "SERVER_SOFTWARE=").c_str());
    if (_request->getXSecret().compare("")) // TO DO assez crade ma foi
        _cgi_env[X_SECRET] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	else
        _cgi_env[X_SECRET] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=");
    if (_request->getTransferEncoding() == "chunked" || _request->getTransferEncoding() == "chunked\r") // TO DO why the \r
        _cgi_env[CONTENT_LENGTH] = ft_strdup("CONTENT_LENGTH=");
    else
        _cgi_env[CONTENT_LENGTH] = ft_strdup(_cgi._content_length.insert(0, "CONTENT_LENGTH=").c_str());
    _cgi_env[NB_METAVARIABLES] = NULL;
}


void	CgiResponse::handleStreamRead(BufferChain& readChain, BufferChain& writeChain)
{
	
	int ret;
	size_t pos;
                // std::cout << "dsddddddddddddddddddddddddddd Nothing more to see here" << std::endl;

	if (_headersReceived == false)
	{
		try
		{
			ret = BufferChain::readToBuffer(_streamReadChain, _streamReadFd);
			if (ret == 0)
			{
				_state.readStream = DONE;
                close(_streamReadFd);
				return;
			}
			_streamBuffer.append(*_streamReadChain.getFirst());
			delete _streamReadChain.getFirst(); // TODO not the most optimized
			_streamReadChain.popFirst();
		}
		catch(const std::exception& e)
		{
			throw;
		}
		if ((pos = _streamBuffer.find("\r\n\r\n")) != std::string::npos) // TO DO add join mechanic if buffer is incomplete
		{
			std::string *httpHeaders;
			std::string *cgiHeaders = new std::string();
			
			cgiHeaders->reserve(pos + 4);
			cgiHeaders->append(_streamBuffer, 0, pos);
			cgiHeaders->append("\r\n\r\n");
			
			httpHeaders = getRawHeaders(); // TO DO make a funciton to add unknonw header s too
			httpHeaders->erase(httpHeaders->size() - 2);

			writeChain.pushBack(httpHeaders);
			writeChain.pushBack(cgiHeaders);
            std::string *body;

            if (_streamBuffer.size() > pos + 4)
            {
			    body = Http::chunkify((char*)_streamBuffer.c_str() + pos + 4 , _streamBuffer.size() - pos - 4);
                writeChain.pushBack(body);
            }
			_streamBuffer.clear();
		}
		_headersReceived = true;
	}
	else
	{
		try
		{
			HttpResponse::handleStreamRead(readChain, writeChain);
		}
		catch(const std::exception& e)
		{
			throw;
		}
	}
}

std::string* CgiResponse::getRawHeaders()
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

        if (_contentLength == -1)
            buff->append("Content-Length: 0\r\n");
        buff->append("Allow: ");
        extension = _route.find_last_of('.');
        it = _config->getCGI_allow(_location).begin();
        itEnd = _config->getCGI_allow(_location).end();
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


void	CgiResponse::abort()
{
	HttpResponse::abort();
	if (_cgipid != -1)
		kill(_cgipid, SIGKILL);
}

CgiResponse::~CgiResponse()
{
	if (_cgipid != -1)
		kill(_cgipid, SIGKILL);
	for (size_t i = 0; i < NB_METAVARIABLES; i++)
		free(_cgi_env[i]);
}