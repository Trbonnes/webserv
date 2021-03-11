
// Set the response to upload a file
void            HttpResponse::get(bool include_body = true)
{
    int         fd;
    struct stat file;


    // Try to get stat of route
    // if it returns anything other than 0, assume it's not found
    if (stat(_route.c_str(), &file))
    {
        _statusCode = NOT_FOUND;
        return;
    }

    // if it's a directory try to autoindex
    if (S_ISDIR(file.st_mode))
    {
        if (_config.getAutoindex(_location) == true)
            autoIndex();
        else
            _statusCode = UNAUTHORIZED; // TO DO not sure about that
        return;
    }

    // Else it's a regular file
    // Check if file exist
    if ((fd = open(_route.c_str(), O_RDONLY)) != -1)
    {
        setLastModified(&file);
        setContentType();
        setCharset();
        _contentLength = file.st_size;
        setServerName();
        setContentLocation();
        _stream_read = fd;
    }
    else 
        _statusCode = INTERNAL_SERVER_ERROR;

    // Remove the body if it's a head request
    if (include_body == false)
    {
        if (_body)
        {
            delete _body;
            _body = NULL;
        }
        if (_stream_read != -1)
        {
            close(_stream_read);
            _stream_read = -1;
        }
    }   
}

void            HttpResponse::del()
{
    struct stat file;

    // Try to get stat of route
    // if it returns anything other than 0, assume it's not found
    if (stat(_route.c_str(), &file))
    {
        _statusCode = NOT_FOUND;
        return;
    }

    int ret = -1;
    //try to unlink file
    if ((file.st_mode & S_IFMT) == S_IFREG)
        ret = unlink(_route.c_str());
    // try to rm dir
    else if (((file.st_mode & S_IFMT) == S_IFDIR))
        ret = rmdir(_route.c_str());
    // if one of those fails no content
    if (ret == -1)
        _statusCode = NO_CONTENT;
}

// Use to download file
void            HttpResponse::put()
{
    struct stat stats;
    stat(_route.c_str(), &stats);

    // Getting path root and appending the file's name
    std::string file = _uri.substr(_location.substr(0, _location.length() - 1).length(), _uri.length());
    _route.assign(_config.getPutRoot()).append(file);
    
    // opening the ouputStream
    // trying to open this existing file with O_TRUNCK to erase content while writing
    _stream_write = open(_route.c_str(), O_WRONLY | O_TRUNC);
    // if -1 then it doesn't exist
    if (_stream_write == -1)
    {
        std::cout << "FILE DOES NOT EXISTS" << std::endl;
        _statusCode = NO_CONTENT; // Should be 201 but the tester expect 204
        // Trying to create the file then
        _stream_write = open(_route.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (_stream_write == -1)
            _statusCode = INTERNAL_SERVER_ERROR;
    }
    else
    {
        std::cout << "FILE ALREADY EXISTS" << std::endl;
        _statusCode = NO_CONTENT;
    }
    setContentLocation();
}

void            HttpResponse::options()
{
    // TO DO
}

void            HttpResponse::cgi()
{
        _use_cgi = true;

        // If it's a head request, just stop there
        // TO DO is this right ?
        if (_request->getMethod().compare("HEAD") != 0) // TO DO string comparison is so gross
        {

        }
}

void        HttpResponse::error()
{
    int         fd;

    _route = _config.getHTMLErrorPage(_statusCode);
    fd = open(_route.c_str(), O_RDONLY);
    if (fd == -1)
    {
        _body = new std::string();
        _body->append("<!DOCTYPE html>\n<html>\n<body>\n\n<h1>");
        char *tmp = ft_itoa(_statusCode);
        _body->append(tmp).append(" ").append(_mapCodes.codes[_statusCode]);
        free(tmp);
        _body->append("</h1>\n\n</body>\n</html>\n");
        _contentType = "text/html";
        _charset = "utf-8";
        _contentLength = _body->length();
    }
    else
    {
        close(fd);
        get(true);
    }
}

void        HttpResponse::post()
{
    std::cout << "Here" << std::endl;
    _contentLength = 0;
}