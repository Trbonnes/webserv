#if !defined(HTTPCONFPARSER)
#define HTTPCONFPARSER

#include "HttpConf.hpp"

#include <string>

class HttpConfParser
{
public:
	HttpConfParser();
	HttpConfParser(HttpConfParser &&) = default;
	HttpConfParser(const HttpConfParser &) = default;
	HttpConfParser &operator=(HttpConfParser &&) = default;
	HttpConfParser &operator=(const HttpConfParser &) = default;
	~HttpConfParser();

private:

public:
	static HttpConf parse(std::string filename);
	
};
#endif // HTTPCONFPARSER
