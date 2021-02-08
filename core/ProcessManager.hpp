#if !defined(PROCESSMANAGER)
#define PROCESSMANAGER

#include "Runnable.hpp"
#include "Log.hpp"
#include "HttpServer.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <signal.h>


class ProcessManager
{
private:
	std::list<Runnable*>		_runnables;
	std::map<pid_t, Runnable*>	_process;
public:
	ProcessManager();
	ProcessManager(const ProcessManager &);
	ProcessManager &operator=(const ProcessManager &);
	~ProcessManager();
	void run(Runnable &proc, unsigned int i, bool clone);
	void manage();
	void killProcesses(int);
};


void	sig_handler(int sig);


#endif // PROCESSMANAGER
