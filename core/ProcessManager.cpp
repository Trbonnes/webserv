#include "ProcessManager.hpp"

ProcessManager::ProcessManager() {
}

ProcessManager::ProcessManager(const ProcessManager &) {
}

ProcessManager &ProcessManager::operator=(const ProcessManager &) {
	return *this;
}

ProcessManager::~ProcessManager() {   
}

void ProcessManager::run(Runnable &proc, unsigned int n = 1)
{
	pid_t pid;

	if (n  == 0)
		return;

	Runnable *cpy = proc.clone();
	while (n-- > 0)
	{	
		if (cpy->isDetached())
		{
			pid = fork();
			std::cout << pid << std::endl;
			if (pid == 0)
			{
				try
				{
					cpy->run();
			Log::debug("ok");
				}
				catch(const std::exception& e)
				{
					std::cerr << "Process error :" << e.what() << '\n';
				}
				delete cpy;
				std::exit(0);
			}
			if (pid == -1)
			{
				// TO DO throw error
			}
			_process[pid] = cpy;
		}
		else
		{
			cpy->run();
			delete cpy;
			// manage errors
		}
		
	}
}

void ProcessManager::manage()
{
	pid_t pid;
	int status;

	while (1 && _process.size() > 0)
	{
		pid = wait(&status);
		std::cout << "Process with pid " << pid << " was killed" << std::endl;
		(void) status;
		// TO DO check error codes
		Runnable* proc = _process[pid];
		if (proc->isRespawn())
		{
			std::cout << "Respawning the process" << std::endl;
			this->run(*proc);
			_process.erase(pid);
			delete proc;
		}
	}
}
