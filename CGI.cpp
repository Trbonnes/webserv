#include "HTTP.hpp"
#include "CGI.hpp"

CGI::CGI() :
_auth_type(""),
_content_length(""),
_content_type(""),
_gateway_interface(""),
_path_info(""),
_path_translated(""),
_query_string(""),
_remote_addr(""),
_remote_ident(""),
_remote_user(""),
_request_method(""),
_request_uri(""),
_script_name(""),
_server_name(""),
_server_port(""),
_server_protocol(""),
_server_software("") {}

CGI::~CGI() {}

void        HTTP::cgi_metaVariables()
{
    std::string str;
    size_t      query;

    _cgi._auth_type = _socket.getAuthorization();
    _cgi._content_length = _socket.getContentLength();
    _cgi._content_type = _socket.getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    query = str.assign(_route).find('?', 0);
    if (query != -1)
        _cgi._path_info = str.erase(query, str.length());
    _cgi._path_translated = _cgi._path_info;
    if (query != -1)
       _cgi._query_string = str.assign(_route).erase(0, query);
    _cgi._remote_addr = "127.0.0.1"; // Default 
    _cgi._remote_ident = ""; // Default
    _cgi._remote_user = ""; // Default
    _cgi._request_method = _socket.getMethod();
    _cgi._request_uri = _socket.getRequestURI();
    query = str.assign(_config.getCGI_root(_location)).find_last_of('/');
    _cgi._script_name = str.erase(0, query);
    _cgi._server_name = _config.getServerName();
    _cgi._server_port = _config.getPort();
    _cgi._server_protocol = "HTTP/1.1";
    _cgi._server_software = "SuperServer/1.0";
    setEnv();
    return ;
}

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
}

int         HTTP::is_good_exe(std::string exe)
{
    std::vector<std::string>::iterator cgi_begin;
    std::vector<std::string>::iterator cgi_end;

    cgi_begin = _config.getCGI(_location).begin();
    cgi_end = _config.getCGI(_location).end();
    while (cgi_begin != cgi_end)
    {
        if (exe.compare(*cgi_begin) == 0)
            return (1);
        cgi_begin++;
    }
    return (0);
}

void        HTTP::cgi_exe()
{
    int     pid;
    int     status;
    int     fd[2];
    char    *line;
    size_t  space;
    int     ret;

    pipe(fd);
    pid = fork();
    if (pid < 0)
        _statusCode = INTERNAL_SERVER_ERROR;
    else if (pid == 0)
    {
        dup2(fd[SIDE_IN], STDOUT_FILENO);
        execve(_config.getCGI_root(_location).c_str(), NULL, _cgi_env);
    }
    else
    {
        waitpid(-1, &status, 0);
        ret = get_next_line(fd[SIDE_OUT], &line);
        _contentType = line;
        space = _contentType.find(' ');
        _contentType.erase(0, space);
        while ((ret = get_next_line(fd[SIDE_OUT], &line)) > 0)
            _body.append(line);
        close(fd[SIDE_IN]);
        close(fd[SIDE_OUT]);
    }
}
