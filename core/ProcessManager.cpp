#include "ProcessManager.hpp"

ProcessManager::ProcessManager(const ProcessManager &) {
}

ProcessManager &ProcessManager::operator=(const ProcessManager &) {
	return *this;
}

ProcessManager::~ProcessManager() {   
}

pid_t ProcessManager::launchProcess(Runnable &proc) 
{
	pid_t pid;

	if (proc._detached)
	{
		pid = fork();
		switch (pid)
		{
		case 0:
			proc.run();
			break;
		case -1:
			// ERROR TO THROW
			break;
		default:
			break;
		}
	}
	else
	{
		proc.run();
		return 0;
	}
	return (pid);
}
