#if !defined(HTTPCONF)
#define HTTPCONF

#include <string>

class HttpConf
{
public:
	HttpConf();
	HttpConf(HttpConf &&) = default;
	HttpConf(const HttpConf &) = default;
	HttpConf &operator=(HttpConf &&) = default;
	HttpConf &operator=(const HttpConf &) = default;
	~HttpConf();
	void parse(std::string filemane); 
private:
	
};
#endif // HTTPCONF
