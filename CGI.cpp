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

void        HTTP::cgi()
{
    _cgi._auth_type = _socket.getAuthorization();
    _cgi._content_length = _socket.getContentLength();
    _cgi._content_type = _socket.getContentType();
    _cgi._gateway_interface = "CGI/1.1";
    _cgi._path_info = _route;
    return ;
}