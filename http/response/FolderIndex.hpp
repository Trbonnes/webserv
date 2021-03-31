#if !defined(FOLDERINDEX_HPP)
#define FOLDERINDEX_HPP

#include <stack>
#include <sys/types.h>
#include <dirent.h>

#include "HttpResponse.hpp"


class FolderIndex : public HttpResponse
{
public:
	FolderIndex(ResponseContext& ctx, BufferChain&, struct stat*);
	~FolderIndex();
};

#endif // FOLDERINDEX_HPP
