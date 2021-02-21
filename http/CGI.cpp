#include "HttpResponse.hpp"
#include "CGI.hpp"

bool        HttpResponse::cgi_fd_exist()
{
    int     fd;

    fd = openFile();
    close(fd);
    if (_statusCode == OK)
        return true;
    return false;
}

//** Check if the method is autorized for the CGI locations **
int         HttpResponse::checkCGImethods(std::string method)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;
    int ret;

    ret = 0;
    itBegin = _config.getCGI_allow(_location).begin();
    itEnd = _config.getCGI_allow(_location).end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            ret = 1;
        itBegin++;
    }
    if (!ret)
        _statusCode = METHOD_NOT_ALLOWED;
    return (ret);
}

// ** Define the meta variables for the CGI script **
void        HttpResponse::cgi_metaVariables()
{
    std::string str;
    size_t      query;

    _cgi._redirect_status = "200";
    _cgi._auth_type = _socket.getAuthorization(); 
    _cgi._content_length = _socket.getContentLength();
    _cgi._content_type = _socket.getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    query = str.assign(_route).find('?', 0);
    _cgi._path_info = _socket.getRequestURI();
    _cgi._path_translated = _route;
    if (query == str.npos)
       _cgi._query_string = str.assign(_route).erase(0, query);
    _cgi._remote_addr = "127.0.0.1";
    _cgi._remote_ident = "login_user";
    _cgi._remote_user = "user";
    _cgi._request_method = _socket.getMethod();
    _cgi._request_uri = _socket.getRequestURI();
    _cgi._script_name = _config.getCGI_root(_location);
    _cgi._server_name = _socket.getHost();
    char *tmp = ft_itoa(_socket.getPort());
    _cgi._server_port = tmp;
    free(tmp);
    _cgi._server_protocol = _config.getHttpVersion();
    _cgi._server_software = _config.getServerSoftware();
    return ;
}

// ** Set the environnement variables in a char** table **
void        HttpResponse::setEnv()
{
    _cgi_env[REDIRECT_STATUS] = ft_strdup(_cgi._redirect_status.insert(0, "REDIRECT_STATUS=").c_str());
    _cgi_env[AUTH_TYPE] = ft_strdup(_cgi._auth_type.insert(0, "AUTH_TYPE=").c_str());
    _cgi_env[CONTENT_LENGTH] = ft_strdup(_cgi._content_length.insert(0, "CONTENT_LENGTH=").c_str());
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
    if (_socket.getXSecret().compare(""))
        _cgi_env[X_SECRET] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
    _cgi_env[NB_METAVARIABLES] = NULL;
}

// ** Verify if the extensions correspond to the config file (CGI) ** 
int         HttpResponse::is_good_exe(std::string exe)
{
    std::vector<std::string>::iterator cgi_begin;
    std::vector<std::string>::iterator cgi_end;

    if (_config.getCGI_root(_location).length() > 0)
    {
        if (_config.getCGI(_location).empty() == false)
        {
            cgi_begin = _config.getCGI(_location).begin();
            cgi_end = _config.getCGI(_location).end();
            while (cgi_begin != cgi_end)
            {
                if (exe.compare(*cgi_begin) == 0)
                    return (1);
                cgi_begin++;
            }
        }
    }
    return (0);
}

// ** Function for the std::adjacent_find, to find the first two \n \r in CGI response and separate the body from the headers **
bool        HttpResponse::mypred(char val1, char val2)
{
    if (val1 == '\n' && val2 == '\r')
        return true;
    return false;
}

// ** Execute the CGI script and get the body and headers **
void        HttpResponse::cgi_exe()
{
    int         pid;
    int         ret;
    char*       args[2];
    std::string route;

    ret = EXIT_SUCCESS;
    ft_bzero(args, sizeof(args));

    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
            Log::debug("CGI launched\n");

        close(_cgi_in[SIDE_IN]);
        close(_cgi_out[SIDE_OUT]);
        dup2(_cgi_in[SIDE_OUT], STDIN_FILENO);
        dup2(_cgi_out[SIDE_IN], STDOUT_FILENO);
        args[0] = ft_strdup(_config.getCGI_root(_location).c_str());
        if ((ret = execve(args[0], args, _cgi_env)) == -1)
        {
            Log::debug("CGI launch error\n");
            exit(EXIT_FAILURE);
        }
        // TO DO test for bad gateway just in case
    }
    else
    {
        close(_cgi_in[SIDE_OUT]);
        close(_cgi_out[SIDE_IN]);
    }
}

void        HttpResponse::cgi_parse()
{
    size_t      find;

    find = _cgiResponse.find("\r\n\r\n");
    if (find != _cgiResponse.npos)
        _contentLength = _responseSize - find - 4;
    find = _cgiResponse.find("Status: ");
    if (find != _cgiResponse.npos)
        _statusCode = ft_atoi(_cgiResponse.substr(find + ft_strlen("Status: "), find + ft_strlen("Status: ") + 3).c_str());
    find = _cgiResponse.find("Content-Type: ");
    if (find != _cgiResponse.npos)
        _contentType = _cgiResponse.substr(find + ft_strlen("Content-Type: "), _cgiResponse.find("\r\n", find) - find - ft_strlen("Content-Type: "));
}


void            HttpResponse::prepare_cgi()
{
    cgi_metaVariables();
    setEnv();
    cgi_exe();
}

void            HttpResponse::read_cgi_response()
{
    int ret;
    char        buf[1024];

    ft_bzero(buf, 1024);
    std::string().swap(_socket.getBody());
    LoadController::loadController(_socket.getContentLength() + 100, _cgiResponse);
    while ((ret = read(_cgi_out[SIDE_OUT], buf, 1024)) > 0) {
        _responseSize += ret;
        buf[ret] = '\0';
        _cgiResponse.append(buf);
    }

     if (ret == -1)
        _statusCode = BAD_GATEWAY;

    close(_cgi_in[SIDE_IN]);
    close(_cgi_out[SIDE_OUT]);
    // if (WIFEXITED(status))
    //     ret = WEXITSTATUS(status);
    // if (ret != EXIT_SUCCESS)
    //     _statusCode = BAD_GATEWAY;
    cgi_parse();
    setDate();
	processResponse();
}

int           HttpResponse::write_cgi_request()
{
    int ret;

    Log::debug("About to write to CGI");
    ret = write(_cgi_in[SIDE_IN], _socket.getBody().c_str(), _socket.getContentLength());
    Log::debug("Wrote to CGI");
     _socket.getBody().clear();
    return ret;
}

bool          HttpResponse::use_cgi()
{
    return _use_cgi;
}
