#if !defined(ERROR_HPP)
#define ERROR_HPP

#include "HttpResponse.hpp"

class Error : public HttpResponse
{
public:
	Error(ConfigServer*, HttpRequest*, BufferChain&, int);
	~Error();
};

#endif // ERROR_HPP
