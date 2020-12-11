#ifndef STATUSCODES_HPP
# define STATUSCODES_HPP

# include <map>
# include <iostream>

// INFORMATIONAL

# define CONTINUE                           100
# define SWITCHING_PROTOCOLS                101

// SUCCESS

# define OK                                 200
# define CREATED                            201
# define ACCEPTED                           202
# define NON_AUTHORITATIVE_INFORMATION      203
# define NO_CONTENT                         204
# define RESET_CONTENT                      205
# define PARTIAL_CONTENT                    206

// REDIRECTION

# define MULTIPLE_CHOICES                   300
# define MOVED_PERMANENTLY                  301
# define FOUND                              302
# define SEE_OTHER                          303
# define NOT_MODIFIED                       304
# define USE_PROXY                          305
# define UNUSED                             306
# define TEMPORARY_REDIRECT                 307

// CLIENT ERROR

# define BAD_REQUEST                        400
# define UNAUTHORIZED                       401
# define PAYMENT_REQUIRED                   402
# define FORBIDDEN                          403
# define NOT_FOUND                          404
# define METHOD_NOT_ALLOWED                 405
# define NOT_ACCEPTABLE                     406
# define PROXY_AUTHENTICATION_REQUIRED      407
# define REQUEST_TIMEOUT                    408
# define CONFLICT                           409
# define GONE                               410
# define LENGTH_REQUIRED                    411
# define PRECONFITION_FAILED                412
# define REQUEST_ENTITY_TOO_LARGE           413
# define REQUEST_URL_TOO_LONG               414
# define UNSUPPORTED_MEDIA_TYPE             415
# define REQUESTED_RANGE_NOT_SATISFIABLE    416
# define EXPECTATION_FAILED                 417

// SERVER ERROR

# define INTERNAL_SERVER_ERROR              500
# define NOT_IMPLEMENTED                    501
# define BAD_GATEWAY                        502
# define SERVICE_UNAVAILABLE                503
# define GATEWAY_TIMEOUT                    504
# define HTTP_VERSION_NOT_SUPPORTED         505 

class  StatusCode
{
    public:
    std::map<int, std::string> codes;

    StatusCode() {
        codes[CONTINUE] = "Continue";
        codes[SWITCHING_PROTOCOLS] = "Switching Protocols";
        codes[OK] = "OK";
        codes[CREATED] = "Created";
        codes[ACCEPTED] = "Accepted";
        codes[NON_AUTHORITATIVE_INFORMATION] = "Non Authoritative Information";
        codes[NO_CONTENT] = "No Content";
        codes[RESET_CONTENT] = "Reset Content";
        codes[PARTIAL_CONTENT] = "Partial Content";
        codes[MULTIPLE_CHOICES] = "Multiple Choices";
        codes[MOVED_PERMANENTLY] = "Moved Permanently";
        codes[FOUND] = "Found";
        codes[SEE_OTHER] = "See Other";
        codes[NOT_MODIFIED] = "Not Modified";
        codes[USE_PROXY] = "Use Proxy";
        codes[UNUSED] = "Unused";
        codes[TEMPORARY_REDIRECT] = "Temporary Redirect";
        codes[BAD_REQUEST] = "Bad Request";
        codes[UNAUTHORIZED] = "Unauthorized";
        codes[PAYMENT_REQUIRED] = "Payment Required";
        codes[FORBIDDEN] = "Forbidden";
        codes[NOT_FOUND] = "Not Found";
        codes[METHOD_NOT_ALLOWED] = "Method Not Allowed";
        codes[NOT_ACCEPTABLE] = "Not Acceptable";
        codes[PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
        codes[REQUEST_TIMEOUT] = "Request Timeout";
        codes[CONFLICT] = "Conflict";
        codes[GONE] = "Gone";
        codes[LENGTH_REQUIRED] = "Length Required";
        codes[PRECONFITION_FAILED] = "Preconfition Failed";
        codes[REQUEST_ENTITY_TOO_LARGE] = "Request Entity Too Large";
        codes[REQUEST_URL_TOO_LONG] = "Request Url Too Long";
        codes[REQUESTED_RANGE_NOT_SATISFIABLE] = "Requested Range Not Satisfiable";
        codes[EXPECTATION_FAILED] = "Expectation Failed";
        codes[INTERNAL_SERVER_ERROR] = "Internal Server Error";
        codes[NOT_IMPLEMENTED] = "Not Implemented";
        codes[BAD_GATEWAY] = "Bad Gateway";
        codes[SERVICE_UNAVAILABLE] = "Service Unvailable";
        codes[GATEWAY_TIMEOUT] = "Gateway Timeout";
        codes[HTTP_VERSION_NOT_SUPPORTED] = "Http Version Not Supported";
    }
};


#endif
