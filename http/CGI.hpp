#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>


enum cgi_variables
{
    REDIRECT_STATUS,
    AUTH_TYPE,
    CONTENT_TYPE,
    GATEWAY_INTERFACE,
    PATH_INFO,
    PATH_TRANSLATED,
    QUERY_STRING,
    REMOTE_ADDR,
    REMOTE_IDENT,
    REMOTE_USER,
    REQUEST_METHOD,
    REQUEST_URI,
    SCRIPT_NAME,
    SERVER_NAME,
    SERVER_PORT,
    SERVER_PROTOCOL,
    SERVER_SOFTWARE,
    X_SECRET,
    CONTENT_LENGTH,
    NB_METAVARIABLES
};


# define SIDE_OUT 0
# define SIDE_IN 1

struct              CGI
{
    std::string     _redirect_status;
    std::string     _auth_type;
    std::string     _content_type;
    std::string     _gateway_interface;
    std::string     _path_info;
    std::string     _path_translated;
    std::string     _query_string;
    std::string     _remote_addr;
    std::string     _remote_ident;
    std::string     _remote_user;
    std::string     _request_method;
    std::string     _request_uri;
    std::string     _script_name;
    std::string     _server_name;
    std::string     _server_port;
    std::string     _server_protocol;
    std::string     _server_software;
    std::string     _content_length;

};

#endif
