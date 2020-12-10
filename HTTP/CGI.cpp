#include "HTTP.hpp"
#include "CGI.hpp"

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

    _cgi._auth_type = _socket.getAuthorization();
    _cgi._content_length = _socket.getContentLength();
    std::cerr << "SOCKET CONTENT LENGTH: " << _socket.getContentLength() << std::endl;
    _cgi._content_length = "100000000";
    _cgi._content_type = _socket.getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    query = str.assign(_route).find('?', 0);
    //_cgi._path_info = _config.getCGI_root(_location);
    _cgi._path_info = "/directory/youpi.bla";
    //_cgi._path_translated = _cgi._path_info;
    _cgi._path_translated = "/YoupiBanane/youpi.bla";
    if (query == str.npos)
       _cgi._query_string = str.assign(_route).erase(0, query);
    _cgi._remote_addr = "127.0.0.1"; // EN DUR! TO DO
    // _cgi._remote_addr = _socket.getRemoteAddr();
    _cgi._remote_ident = "user"; // Default
    _cgi._remote_user = "user"; // Default 
    _cgi._request_method = _socket.getMethod();
    _cgi._request_uri = _socket.getRequestURI();
    _cgi._script_name = "/directory/youpi.bla";
    //_cgi._script_name = _config.getCGI_root(_location);
    _cgi._server_name = _config.getServerName()[0]; // TO DO quick fix
    _cgi._server_port = ft_itoa(_config.getPort()[0]); // TO DO fix getPort()
    _cgi._server_protocol = _config.getHttpVersion();
    _cgi._server_software = _config.getServerSoftware();
    setEnv();
    return ;
}

# include <stdio.h> // TEST

// ** Set the environnement variables in a char** table **
void        HTTP::setEnv()
{
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
    _cgi_env[NB_METAVARIABLES] = NULL;
    int i = 0;
    while (i < NB_METAVARIABLES)
        printf("%s\n", _cgi_env[i++]);
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
    int         fd[2];
    int         status;
    char*       args[2];
    char        buf[2048];
    int         mem;
    size_t      find;

    ret = 0;
    ft_bzero(args, sizeof(args));
    ft_bzero(buf, sizeof(args));
    ft_bzero(fd, sizeof(fd));
    pipe(fd);
    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
        args[0] = ft_strdup(_config.getCGI_root(_location).c_str());
        args[1] = NULL;
        dup2(fd[SIDE_IN], STDOUT_FILENO);
        dup2(fd[SIDE_OUT], STDIN_FILENO);
        write(STDOUT_FILENO, _socket.getBody().c_str(), ft_atoi(_socket.getContentLength().c_str()));
        if ((ret = execve(args[0], args, _cgi_env)) == -1)
            exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(-1, &status, 0);
        if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
        close(fd[SIDE_IN]);
        if (ret == EXIT_SUCCESS)
        {
            mem = 0;
            find = _cgiResponse.npos;
            while ((ret = read(fd[SIDE_OUT], buf, sizeof(buf))) != 0)
            {
                _responseSize += ret;
                if (find == _cgiResponse.npos)
                {
                    _cgiResponse.append(buf);
                    find = _cgiResponse.find("\r\n\r\n");
                }
                if (find != _cgiResponse.npos)
                {
                    if (mem == 0)
                    {                   
                        mem = _responseSize;
                        _body = (char*)ft_calloc(mem, sizeof(char));
                        _contentLength = 0;
                        _contentLength += _cgiResponse.length() - find - 4;
                        _body = ft_memcat(_body, _cgiResponse.substr(find + 4, _cgiResponse.length()).c_str(), _cgiResponse.length() - find - 4);
                    }
                    else if (mem < _responseSize)
                    {
                       mem *= 2;
                        _body = (char*)ft_realloc(_body, sizeof(char) * mem);
                        _contentLength += ret;
                        _body = ft_memcat(_body, buf, ret);
                    }
                    _cgiResponse.erase(find + 2, _cgiResponse.length());
                }
            }
            close(fd[SIDE_OUT]);
        }
        else
            _statusCode = BAD_GATEWAY;
    }
    find = _cgiResponse.find("Status: ");
    _statusCode = ft_atoi(_cgiResponse.substr(find + ft_strlen("Status: "), find + ft_strlen("Status: ") + 3).c_str());
    find = _cgiResponse.find("Content-Type: ");
    _contentType = _cgiResponse.substr(find + ft_strlen("Content-Type: "), _cgiResponse.find("\r\n", find) - find - ft_strlen("Content-Type: "));
}
