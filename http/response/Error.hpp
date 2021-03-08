#if !defined(ERROR_HPP)
#define ERROR_HPP

#include "HttpResponse.hpp"

class Error : HttpResponse
{
public:
	Error(ConfigServer*, HttpRequest*, int);
	~Error();
};

#endif // ERROR_HPP
