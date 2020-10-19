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
	fd_set _listen_sockset;
	fd_set _active_sockset;
	bool is_on_listen_sockset();
public:
	void run() override;


};
#endif // HTTPWORKER