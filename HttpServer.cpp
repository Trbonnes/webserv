#include "HttpServer.hpp"


HttpServer::HttpServer() {
    
}

HttpServer::~HttpServer() {
    
}

void HttpServer::initConf() {
    std::cout << "Initializing configuration" << std::endl;
}

void HttpServer::initListenSocket() {
	std::cout << "Initializing listening sockets" << std::endl;
}

void HttpServer::initWorkers() {
    std::cout << "Initializing workers" << std::endl;
}
