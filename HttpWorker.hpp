#if !defined(HTTPWORKER)
#define HTTPWORKER

#include <iostream>

#include "Runnable.hpp"

class HttpWorker : public Runnable
{
public:
	HttpWorker();
	HttpWorker(HttpWorker &&) = default;
	HttpWorker(const HttpWorker &) = default;
	HttpWorker &operator=(HttpWorker &&) = default;
	HttpWorker &operator=(const HttpWorker &) = default;
	~HttpWorker();

private:

public:
	void run();

};
#endif // HTTPWORKER