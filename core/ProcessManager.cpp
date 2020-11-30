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
			try
			{
				proc.run();
			}
			catch(const std::exception& e)
			{
				std::cerr << "Process error :" << e.what() << '\n';
			}
			std::exit(0);
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
