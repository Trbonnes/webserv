#include "../HttpResponse.hpp"



// First it opens the file, 
void        HttpResponse::getInit()
{
    _stream_out = openFile();
}

void        HttpResponse::get()
{
    std::string root;

    authorization();
    if (_statusCode == OK)
    {
        setBody(_stream_out);
        setLastModified();
        setContentType();
        setCharset();
        setContentLength();
        setServerName();
        setContentLocation();
        setDate();
    }
    else if (_statusCode != UNAUTHORIZED && _config.getAutoindex(_location) == true)
    {
        setAutoindex();
        setDate();
        _contentLanguage = "";
        _statusCode = OK;
    }
    return ;
}

//** Define the language page **
std::string     HttpResponse::acceptLanguage()
{
    std::vector<std::string>::iterator itClientBegin;
    std::vector<std::string>::iterator itClientEnd;
    std::vector<std::string>::iterator itServer;
    std::vector<std::string>::iterator itServerEnd;
    std::string str;
    std::string trydir;
    struct stat dir;
    int r;

    if (!_config.getLanguage(_location).empty())
    {
        str.assign("/");
        itClientBegin = _socket.getAcceptLanguage().begin();
        itClientEnd = _socket.getAcceptLanguage().end();
        itServerEnd = _config.getLanguage(_location).end();
        while (itClientBegin != itClientEnd)
        {
            itServer = std::find(_config.getLanguage(_location).begin(), _config.getLanguage(_location).end(), *itClientBegin);
            if (itServer != itServerEnd)
            {
                _contentLanguage = *itServer;
                str.assign(*itServer);
                str.append("/");
                r = stat(trydir.assign(_route).append(str).c_str(), &dir);
                if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
                    return (str);
                else
                    _contentLanguage.assign("");
            }
            itClientBegin++;
        }
        _contentLanguage = *(_config.getLanguage(_location).begin());
        str.append(*(_config.getLanguage(_uri).begin()));
        r = stat(trydir.assign(_route).append(str).c_str(), &dir);
        if (r != -1 && (dir.st_mode & S_IFMT) == S_IFDIR)
            return (str);
        else
            return (_contentLanguage.assign(""));
    }
    return ("");
}


// ** Create the default html page when file is not found and autoindex is on **
void            HttpResponse::setAutoindex(void)
{
    std::string             str;
    struct stat             directory;
    DIR                     *dir;
    struct dirent           *dirent;
    struct tm               *timeinfo;
    char                    lastModifications[100];
    std::stack<std::string> files;
    std::string             body;

    body.assign("<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n");
    dir = opendir(_route.c_str());
    if (dir == NULL)
        _statusCode = INTERNAL_SERVER_ERROR;
    else
    {
        while ((dirent = readdir(dir)) != NULL)
        {
            stat(str.assign(_uri).append(dirent->d_name).c_str(), &directory);
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
            	timeinfo = localtime(&(directory.st_mtim.tv_sec));
			#else
            	timeinfo = localtime(&(directory.st_mtimespec.tv_sec));
			#endif // TARGET_OS_MAC
			
            strftime(lastModifications, 100, "%d-%b-20%y %OH:%OM", timeinfo);
            str.append(lastModifications);
            str.append("\t\t");
            if (dirent->d_type == DT_DIR)
                str.append("-");
            else
            {
                char *dirSize = ft_itoa(directory.st_size);
                str.append(dirSize);
                free(dirSize);
            }
            str.append("\n");
            files.push(str);
        }
        while (!files.empty())
        {
            body.append(files.top());
            files.pop();
        }
    }
    closedir(dir);
    body.append("</pre><hr></body>\n</html>");
    _contentLength = body.length();
    _contentType = "text/html";
    _charset = "utf-8";
    _body = (char*)ft_calloc(_contentLength + 1, sizeof(char));
    ft_strcpy(_body, body.c_str());
}