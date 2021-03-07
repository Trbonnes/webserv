#include "HttpResponse.hpp"
#include "CGI.hpp"

//** Check if the method is autorized for the CGI locations **
int         HttpResponse::checkCGImethods(std::string method)
{
    std::vector<std::string>::iterator itBegin;
    std::vector<std::string>::iterator itEnd;

    itBegin = _config.getCGI_allow(_location).begin();
    itEnd = _config.getCGI_allow(_location).end();
    while (itBegin != itEnd)
    {
        _allow.push_back(*itBegin);
        if ((*itBegin).compare(method) == 0)
            return 1;
        itBegin++;
    }
    return 0;
}

// ** Define the meta variables for the CGI script **
void        HttpResponse::cgi_metaVariables()
{
    std::string str;
    size_t      query;

    _cgi._redirect_status = "200";
    _cgi._auth_type = _request->getAuthorization(); 
    _cgi._content_length = _request->getContentLength();
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
    _cgi._script_name = _config.getCGI_root(_location);
    _cgi._server_name = _request->getHost();
    char *tmp = ft_itoa(_request->getPort());
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
    if (_request->getXSecret().compare(""))
        _cgi_env[X_SECRET] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
    if (_request->getTransferEncoding() == "chunked\r") // TO DO why the \r
        _cgi_env[CONTENT_LENGTH] = ft_strdup(_cgi._content_length.insert(0, "CONTENT_LENGTH=").c_str());
    _cgi_env[NB_METAVARIABLES] = NULL;
}

// ** Verify if the extensions correspond to the config file (CGI) ** 
int         HttpResponse::is_good_exe(std::string exe)
{
    std::vector<std::string>::iterator cgi_begin;
    std::vector<std::string>::iterator cgi_end;

    std::cout << "exe |" << exe << "|" << std::endl;
    std::cout << "Here is the location" << _location << "|" << std::endl;
    std::cout << _config.getCGI_root(_location).length() << std::endl;

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
    char*       args[2];
    int         cgi_in[2];
    int         cgi_out[2];
    std::string route;

    // put args to 0
    ft_bzero(args, sizeof(args));

    // first pipe
    if (pipe(cgi_in))
    {
        _statusCode = INTERNAL_SERVER_ERROR;
        return;
    }

    // second pipe
    if (pipe(cgi_out))
    {
        close(cgi_in[0]);
        close(cgi_in[1]);
        _statusCode = INTERNAL_SERVER_ERROR;
        return;
    }
    // Fork
    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
        Log::debug("CGI launched\n");

        // Closing unused end in chlid process
        close(cgi_in[SIDE_IN]);
        close(cgi_out[SIDE_OUT]);
        // Redirecting the other pipe to stdin and stdout
        dup2(cgi_in[SIDE_OUT], STDIN_FILENO);
        dup2(cgi_out[SIDE_IN], STDOUT_FILENO);

        

        args[0] = ft_strdup(_config.getCGI_root(_location).c_str());
        if (execve(args[0], args, _cgi_env) == -1)
        {
            Log::debug("CGI launch error");
            exit(EXIT_FAILURE);
        }
        // TO DO test for bad gateway just in case
    }
    else
    {
        sleep(1);
        // closing unused pipe in parent and storing fd fd in streams
        close(cgi_in[SIDE_OUT]);
        close(cgi_out[SIDE_IN]);
        _stream_write = cgi_in[SIDE_IN];
        _stream_read = cgi_out[SIDE_OUT];
        std::cout << "==========>    fd is " << _stream_write << std::endl;
        fcntl(_stream_write, F_SETFL, O_NONBLOCK);
        fcntl(_stream_read, F_SETFL, O_NONBLOCK);
    }
}

bool          HttpResponse::use_cgi()
{
    return _use_cgi;
}
