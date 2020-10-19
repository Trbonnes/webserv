#include "HttpWorker.hpp"


// The runnable arguments might depend from the configuration file
HttpWorker::HttpWorker() : Runnable(1, 1){
	std::cout << "Worker Initializing" << std::endl;
    
}

HttpWorker::~HttpWorker() {
}

#include <unistd.h>
void	HttpWorker::run()
{
	fd_set read_fs;

	std::cout << "Running a worker" << std::endl;
	_active_sockset = _listen_sockset;
	while (1)
	{
		read_fs = _active_sockset;
		if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) == -1) // check if 0
		{
			// WHAT to do ? 
			continue; // throw something
		}
		//Serve every read client
		while ()
		{
			/* code */
		}
		

	}
}


bool HttpWorker::is_on_listen_sockset()
{
	return false;
}
