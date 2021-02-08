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
				g_ismaster = false;
				ptr->run();
				break;
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
        pid = waitpid(-1, &status, 0);
		Runnable* proc = _process[pid];
		if (proc->isRespawn() && g_server->getStatus() != HttpServer::STOPPING)
		{
			std::cerr << "Respawning a worker" << std::endl;
			this->run(*proc, 1, false);
		}
		else
			_process.erase(pid);
	}
}


void ProcessManager::killProcesses(int sig)
{
	for (std::map<pid_t, Runnable*>::iterator it = _process.begin(); it != _process.end(); it++)
		kill(it->first, sig);
}