#if !defined(FILEUPLOAD_HPP)
#define FILEUPLOAD_HPP

#include "http/response/HttpResponse.hpp"

class FileUpload : public HttpResponse
{

	std::string _file;

public:
	FileUpload(ConfigServer*, HttpRequest*, std::string, std::string);
	//Coplien to do
	~FileUpload();

	void handleStreamWrite(BufferChain& readChain, BufferChain& writeChain);
	void handleRead(BufferChain& readChain, BufferChain& writeChain);
	void abort();
};


#endif // FILEDUPLOAD_HPP
