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
    {
        std::cerr << "CGI ALLOW NOT METHOD" << std::endl;
        _statusCode = METHOD_NOT_ALLOWED;
    }
    return (ret);
}

// ** Define the meta variables for the CGI script **
void        HTTP::cgi_metaVariables()
{
    std::string str;
    size_t      query;

    std::cerr << "authorization: " << _socket.getAuthorization() << std::endl;
    std::cerr << "getContentLength: " << _socket.getContentLength() << std::endl;
    std::cerr << "getContentType: " << _socket.getContentType() << std::endl;
    std::cerr << "getMethod: " << _socket.getMethod() << std::endl;
    std::cerr << "getRequestURI: " << _socket.getRequestURI() << std::endl;
    std::cerr << "_config.getPort()[0]: " << _config.getPort()[0] << std::endl;
    std::cerr << "_config.getServerName()[0]: " << _config.getServerName()[0] << std::endl;
    std::cerr << "_config.getHttpVersion(): " << _config.getHttpVersion() << std::endl;
    std::cerr << "_config.getServerSoftware(): " << _config.getServerSoftware() << std::endl;
    _cgi._auth_type = _socket.getAuthorization();
    _cgi._content_length = "22"; //_socket.getContentLength() ;
    _cgi._content_type = _socket.getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    query = str.assign(_route).find('?', 0);
    // if (query != str.npos) // TO DO check if behavior stays the same
        // _cgi._path_info = str.erase(query, str.length());
    // else
        // _cgi._path_info = _route;
    // _cgi._path_translated = _cgi._path_info;
    _cgi._path_info = _socket.getRequestURI();
    _cgi._script_name = _socket.getRequestURI();
    // _cgi._path_translated = _config.getRoot(_location);
    _cgi._path_translated = "/home/pauline/webserver/www/";
    if (query == str.npos)
       _cgi._query_string = str.assign(_route).erase(0, query);
    // _cgi._remote_addr = _socket.getRemoteAddr(); // Default 
    _cgi._remote_addr = "127.0.0.1";
    _cgi._remote_ident = "user"; // Default
    _cgi._remote_user = "user"; // Default 
    _cgi._request_method = _socket.getMethod();
    _cgi._request_uri = _socket.getRequestURI();
    // _cgi._script_name = _config.getCGI_root(_location);
    _cgi._server_name = _config.getServerName()[0]; // TO DO quick fix
    _cgi._server_port = std::to_string(_config.getPort()[0]); // TO DO fix getPort()
    _cgi._server_protocol = _config.getHttpVersion();
    _cgi._server_software = _config.getServerSoftware();
    _cgi._script_filename = _route;
    setEnv();
    return ;
}

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
    _cgi_env[SCRIPT_FILENAME] = ft_strdup(_cgi._script_filename.insert(0, "SCRIPT_FILENAME=").c_str());
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
    std::cerr << "is good exe return 0" << std::endl;
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
    // char        *line;
    char        *args[2];
    char        buf[1024];
    // size_t      space;
    // size_t      find;
    std::string str;
    // struct stat stat;
    // std::string::iterator it;
    std::string response;

    pipe(fd);
    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
        dup2(fd[SIDE_IN], STDOUT_FILENO);
        dup2(fd[SIDE_OUT], STDIN_FILENO);
        write(STDIN_FILENO, _socket.getBody().c_str(), _socket.getBody().length());
        args[0] = ft_strdup(_config.getCGI_root(_location).c_str());
        args[1] = NULL;
        if ((ret = execve(args[0], args, _cgi_env)) == -1)
            exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(-1, &status, 0);
        std::cerr << "END CGI" << std::endl;
        // if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
        close(fd[SIDE_IN]);
        if (ret == EXIT_SUCCESS)
        {
            while ((ret = read(fd[SIDE_OUT], buf, sizeof(buf))) != 0)
            {
                buf[ret] = '\0';
                response.append(buf);
            }
            close(fd[SIDE_OUT]);
            std::cout << std::endl << std::endl << "CGI RESPONSE: " << std::endl << response << std::endl;
            // find = response.find("Status: ");
            // _statusCode = ft_atoi(response.substr(find + 8, 3).c_str());
            // find = response.find("Content-Type: ");
            // _contentType = response.substr(find + 14, response.find('\n', find + 14) - find - 14);
            // it = std::adjacent_find(response.begin(), response.end(), mypred);
            // response.erase(response.begin(), it + 3);
        }
        else
            _statusCode = BAD_GATEWAY;
        std::cerr << "RESPONSE: " << response << std::endl;
        if (response.length() > 0)
            _response = ft_strdup(response.c_str());
    }
}
