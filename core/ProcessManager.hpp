#if !defined(PROCESSMANAGER)
#define PROCESSMANAGER

#include "Runnable.hpp"

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

class ProcessManager
{
public:
	ProcessManager();
	ProcessManager(const ProcessManager &);
	ProcessManager &operator=(const ProcessManager &);
	~ProcessManager();
	static	pid_t launchProcess(Runnable &proc);
};

#endif // PROCESSMANAGER
