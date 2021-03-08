#if !defined(FILEDOWNLOAD_HPP)
#define FILEDOWNLOAD_HPP

#include "http/response/HttpResponse.hpp"

class FileDownload : public HttpResponse
{

public:
	//Coplien to do
	FileDownload(ConfigServer*, HttpRequest*, std::string, std::string, BufferChain& writeChain, struct stat*);
	~FileDownload();
};


#endif // FILEDOWNLOAD_HPP
