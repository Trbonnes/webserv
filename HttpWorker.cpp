#include "HttpWorker.hpp"


HttpWorker::HttpWorker() : Runnable(1, 1){
	std::cout << "Worker Initializing" << std::endl;
    
}

HttpWorker::~HttpWorker() {
}

void	HttpWorker::run()
{
	std::cout << "Running a worker" << std::endl;
}
