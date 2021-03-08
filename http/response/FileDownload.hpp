#if !defined(FILEDOWNLOAD_HPP)
#define FILEDOWNLOAD_HPP

#include "http/response/HttpResponse.hpp"


class FileDownload : public HttpResponse
{

private:

	struct stat *_file;

public:
	//Coplien to do
	FileDownload(ConfigServer*, HttpRequest*, std::string, std::string, struct stat*);
	~FileDownload();

	// Utils
	void        setCharset(void);
	void        setContentType();
	void        setLastModified();
}


#endif // FILEDOWNLOAD_HPP
