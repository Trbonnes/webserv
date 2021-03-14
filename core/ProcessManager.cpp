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

	if (n == 0)
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
		if (errno == ECHILD)
			break;
		Runnable* proc = _process[pid];
		_process[pid] = 0;
		if (proc->isRespawn() && g_server->getStatus() != HttpServer::STOPPING)
		{
			std::cerr << "Respawning a worker" << std::endl;
			sleep(50);
			this->run(*proc, 1, false);
			// If it's a worker exiting
			if (g_ismaster == false)
			{
				break ;
			}
		}
		else
			_process.erase(pid);
	}
}


void ProcessManager::killProcesses(int sig)
{
	std::map<pid_t, Runnable*>::iterator next, it = _process.begin();
	while (it != _process.end())
	{
		// This is to avoid concurrency problem with an entry being erased before next loop iteration
		next = it;
		next++;
		kill(it->first, sig);
		it = next;
	}
}