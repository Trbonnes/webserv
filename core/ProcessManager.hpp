#if !defined(PROCESSMANAGER)
#define PROCESSMANAGER

#include "Runnable.hpp"
#include "Log.hpp"

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


class ProcessManager
{
private:
	std::map<pid_t, Runnable> _process;
public:
	ProcessManager();
	ProcessManager(const ProcessManager &);
	ProcessManager &operator=(const ProcessManager &);
	~ProcessManager();
	void run(Runnable &proc, unsigned int i);
	void manage();
};

#endif // PROCESSMANAGER
