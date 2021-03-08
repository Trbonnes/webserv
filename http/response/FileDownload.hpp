#if !defined(FILEDOWNLOAD_HPP)
#define FILEDOWNLOAD_HPP

#include "http/response/HttpResponse.hpp"


class FileDownload : HttpResponse
{
public:
	//Coplien to do
	FileDownload(std::string);
	~FileDownload();

	void	abort();
	void	handleStreamRead();
}


#endif // FILEDOWNLOAD_HPP
