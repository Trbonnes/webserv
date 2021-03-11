#include "FolderIndex.hpp"


FolderIndex::FolderIndex(
		ConfigServer* config,
		HttpRequest* request,
		std::string& route,
		std::string& location,
		BufferChain &writeChain, struct stat* directory) : HttpResponse(config, request, route, location)
{
	std::string             str;
    DIR                     *dir;
    struct dirent           *dirent;
    struct tm               *timeinfo;
    char                    lastModifications[100];
    std::stack<std::string> files;

    dir = opendir(_route.c_str());

	if (dir == NULL)
	 	throw HttpResponse::HttpError(FORBIDDEN); 
    
	// if there's an se terror to internal server error
	std::string *buff = new std::string();
	buff->append("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
	while ((dirent = readdir(dir)) != NULL)
	{
		stat(str.assign(_route).append(dirent->d_name).c_str(), directory);
		if (str.assign(dirent->d_name).compare(".") == 0)
			continue ;
		str.assign("<a href=\"");
		str.append(dirent->d_name);
		if (dirent->d_type == DT_DIR)
			str.append("/");
		str.append("\">");
		str.append(dirent->d_name);
		if (dirent->d_type == DT_DIR)
			str.append("/");
		str.append("</a>\t\t\t\t");
		#ifdef __linux__
			timeinfo = localtime(&(directory->st_mtim.tv_sec));
		#else
			timeinfo = localtime(&(directory->st_mtimespec.tv_sec));
		#endif // TARGET_OS_MAC
		
		strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
		str.append(lastModifications);
		str.append("\t\t");
		if (dirent->d_type == DT_DIR)
			str.append("-");
		else
		{
			char *dirSize = ft_itoa(directory->st_size);
			str.append(dirSize);
			free(dirSize);
		}
		str.append("\n");
		files.push(str);
	}
	while (!files.empty())
	{
		buff->append(files.top());
		files.pop();
	}
	closedir(dir);
	buff->append("</pre><hr></body>\n</html>");
	_contentType = "text/html";
	_charset = "utf-8";
	_contentLength = buff->length();

	// appending the buffers
	writeChain.pushBack(getRawHeaders());
	writeChain.pushBack(buff);

	// updating the state
	_state.read = DONE;
}

FolderIndex::~FolderIndex()
{
}