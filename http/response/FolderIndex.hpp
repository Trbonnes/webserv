#if !defined(FOLDERINDEX_HPP)
#define FOLDERINDEX_HPP

#include <stack>
#include <sys/types.h>
#include <dirent.h>

#include "HttpResponse.hpp"


class FolderIndex : public HttpResponse
{
public:
	FolderIndex(ConfigServer*, HttpRequest*, std::string &route, std::string &location,BufferChain&, struct stat*);
	~FolderIndex();
};

#endif // FOLDERINDEX_HPP
