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

class ProcessManager
{
public:
	ProcessManager() = default;
	ProcessManager(ProcessManager &&) = default;
	ProcessManager(const ProcessManager &) = default;
	ProcessManager &operator=(ProcessManager &&) = default;
	ProcessManager &operator=(const ProcessManager &) = default;
	~ProcessManager() = default;
	static	pid_t launchProcess(Runnable &proc);
};

#endif // PROCESSMANAGER
