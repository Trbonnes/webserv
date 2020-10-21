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
        codes[CONTINUE] = "CONTINUE";
        codes[SWITCHING_PROTOCOLS] = "SWITCHING_PROTOCOLS";
        codes[OK] = "OK";
        codes[CREATED] = "CREATED";
        codes[ACCEPTED] = "ACCEPTED";
        codes[NON_AUTHORITATIVE_INFORMATION] = "NON_AUTHORITATIVE_INFORMATION";
        codes[NO_CONTENT] = "NO_CONTENT";
        codes[RESET_CONTENT] = "RESET_CONTENT";
        codes[PARTIAL_CONTENT] = "PARTIAL_CONTENT";
        codes[MULTIPLE_CHOICES] = "MULTIPLE_CHOICES";
        codes[MOVED_PERMANENTLY] = "MOVED_PERMANENTLY";
        codes[FOUND] = "FOUND";
        codes[SEE_OTHER] = "SEE_OTHER";
        codes[NOT_MODIFIED] = "NOT_MODIFIED";
        codes[USE_PROXY] = "USE_PROXY";
        codes[UNUSED] = "UNUSED";
        codes[TEMPORARY_REDIRECT] = "TEMPORARY_REDIRECT";
        codes[BAD_REQUEST] = "BAD_REQUEST";
        codes[UNAUTHORIZED] = "UNAUTHORIZED";
        codes[PAYMENT_REQUIRED] = "PAYMENT_REQUIRED";
        codes[FORBIDDEN] = "FORBIDDEN";
        codes[NOT_FOUND] = "NOT_FOUND";
        codes[METHOD_NOT_ALLOWED] = "METHOD_NOT_ALLOWED";
        codes[NOT_ACCEPTABLE] = "NOT_ACCEPTABLE";
        codes[PROXY_AUTHENTICATION_REQUIRED] = "PROXY_AUTHENTICATION_REQUIRED";
        codes[REQUEST_TIMEOUT] = "REQUEST_TIMEOUT";
        codes[CONFLICT] = "CONFLICT";
        codes[GONE] = "GONE";
        codes[LENGTH_REQUIRED] = "LENGTH_REQUIRED";
        codes[PRECONFITION_FAILED] = "PRECONFITION_FAILED";
        codes[REQUEST_ENTITY_TOO_LARGE] = "REQUEST_ENTITY_TOO_LARGE";
        codes[REQUEST_URL_TOO_LONG] = "REQUEST_URL_TOO_LONG";
        codes[REQUESTED_RANGE_NOT_SATISFIABLE] = "REQUESTED_RANGE_NOT_SATISFIABLE";
        codes[EXPECTATION_FAILED] = "EXPECTATION_FAILED";
        codes[INTERNAL_SERVER_ERROR] = "INTERNAL_SERVER_ERROR";
        codes[NOT_IMPLEMENTED] = "NOT_IMPLEMENTED";
        codes[BAD_GATEWAY] = "BAD_GATEWAY";
        codes[SERVICE_UNAVAILABLE] = "SERVICE_UNAVAILABLE";
        codes[GATEWAY_TIMEOUT] = "GATEWAY_TIMEOUT";
        codes[HTTP_VERSION_NOT_SUPPORTED] = "HTTP_VERSION_NOT_SUPPORTED";
    }
};


#endif
