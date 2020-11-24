#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>

struct              CGI
{
    std::string     _auth_type;
    std::string     _content_length;
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

    std::string     _script_filename;
};

#endif
