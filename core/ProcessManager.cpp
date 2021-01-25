#include "ProcessManager.hpp"

ProcessManager::ProcessManager() {
}

ProcessManager::ProcessManager(const ProcessManager &) {
}

ProcessManager &ProcessManager::operator=(const ProcessManager &) {
	return *this;
}

ProcessManager::~ProcessManager() {
	for (std::list<Runnable*>::iterator it = _runnables.begin(); it != _runnables.end(); ++it)
		delete *it;
}


void ProcessManager::run(Runnable &proc, unsigned int n = 1, bool clone = true)
{
	pid_t pid;
	Runnable *ptr;


	if (n  == 0)
		return;

	if (clone)
	{
		ptr = proc.clone();
		_runnables.push_back(ptr);
	}
	else
		ptr = &proc;
	while (n-- > 0)
	{	
		if (ptr->isDetached())
		{
			pid = fork();
			if (pid == 0)
			{
				ptr->run();
			}
			if (pid == -1)
			{
				throw Runnable::RunnableLaunchException();
			}
			_process.erase(pid);
			_process[pid] = ptr;
		}
		else
		{
			try
			{
				ptr->run();
			}
			catch(const std::exception& e)
			{
				std::cerr << "Encountered failure while running process" << e.what() << '\n';
			}	
		}
		
	}
}

void ProcessManager::manage()
{
	pid_t pid = 0;
	int status;

	while (_process.size() > 0)
	{
		// pid = wait(&status);
        waitpid(-1, &status, 0);

		std::cerr << "Dead pid" << pid << std::endl;
		std::cerr << "Was signaled ? " << WIFSIGNALED(status) << std::endl;
		std::cerr << "What code ? " << WTERMSIG(status) << std::endl;
		std::cerr << "Core dump ? " << WCOREDUMP(status) << std::endl;
		std::cerr << "Process with pid " << pid << " was killed" << std::endl;
		Runnable* proc = _process[pid];
		std::cerr << "Value of proc " << proc << std::endl;
		if (proc->isRespawn())
		{
			std::cerr << "Respawning the process" << std::endl;
			// this->run(*proc, 1, false);
		}
	}
}
