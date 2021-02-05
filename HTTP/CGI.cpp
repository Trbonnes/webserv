#include "HTTP.hpp"
#include "CGI.hpp"

bool        HTTP::cgi_fd_exist()
{
    int     fd;

    fd = openFile();
    close(fd);
    if (_statusCode == OK)
        return true;
    return false;
}

//** Check if the method is autorized for the CGI locations **
int         HTTP::checkCGImethods(std::string method)
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
void        HTTP::cgi_metaVariables()
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
void        HTTP::setEnv()
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
int         HTTP::is_good_exe(std::string exe)
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
bool        HTTP::mypred(char val1, char val2)
{
    if (val1 == '\n' && val2 == '\r')
        return true;
    return false;
}

// ** Execute the CGI script and get the body and headers **
void        HTTP::cgi_exe()
{
    int         pid;
    int         ret;
    int         status;
    int         save_stdin;
    int         save_stdout;
    int         side_in[2];
    char        buf[2049];
    char*       args[2];
    int         side_out;
    int         file = 0;
    std::string route;

    save_stdout = dup(STDOUT_FILENO);
    save_stdin = dup(STDIN_FILENO);
    ret = EXIT_SUCCESS;
    ft_bzero(args, sizeof(args));
    ft_bzero(buf, sizeof(args));
    ft_bzero(side_in, sizeof(side_in));
    pipe(side_in);
    route = "HTTP/cgi/";
    char *tmp = ft_itoa(file);
    route.append(tmp);
    free(tmp);
    while ((side_out = open(route.c_str(), O_RDONLY)) != -1)
    {
        close(side_out);
        file++;
        route = "HTTP/cgi/";
        tmp = ft_itoa(file);
        route.append(tmp);
        free(tmp);
    }
    while ((side_out = open(route.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR)) == -1) ;
    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
        close(side_in[SIDE_IN]);
        dup2(side_in[SIDE_OUT], STDIN_FILENO);
        dup2(side_out, STDOUT_FILENO);
        args[0] = ft_strdup(_config.getCGI_root(_location).c_str());
        if ((ret = execve(args[0], args, _cgi_env)) == -1)
            exit(EXIT_FAILURE);
    }
    else
    {
        close(side_out);
        close(side_in[SIDE_OUT]);
        write(side_in[SIDE_IN], _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
        waitpid(-1, &status, 0);
        close(side_in[SIDE_IN]);
        _socket.getBody().clear();
        std::string().swap(_socket.getBody());
	    LoadController::loadController(ft_atoi(_socket.getContentLength().c_str()) + 100, _cgiResponse);
        while ((side_out = open(route.c_str(), O_RDONLY, S_IRUSR | S_IWUSR)) == -1) ;
        while ((ret = read(side_out, buf, 1024)) > 0) {
            _responseSize += ret;
            buf[ret] = '\0';
            _cgiResponse.append(buf);
        }
        close(side_out);
        unlink(route.c_str());
        if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
        if (ret != EXIT_SUCCESS)
            _statusCode = BAD_GATEWAY;
    }
    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stdin, STDIN_FILENO);
}

void        HTTP::cgi_parse()
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
