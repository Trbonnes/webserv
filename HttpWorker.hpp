#if !defined(HTTPWORKER)
#define HTTPWORKER

#include <iostream>
#include <vector>

#include "ListenSocket.hpp"
#include "HttpConnection.hpp"
#include "Runnable.hpp"

class HttpWorker : public Runnable
{
public:
	HttpWorker(std::vector<ListenSocket> &listen);
	HttpWorker(HttpWorker &&) = default;
	HttpWorker(const HttpWorker &) = default;
	HttpWorker &operator=(HttpWorker &&) = default;
	HttpWorker &operator=(const HttpWorker &) = default;
	~HttpWorker();

private:
	std::vector<ListenSocket> _listen_socket;

public:
	void run() override;


};
#endif // HTTPWORKER