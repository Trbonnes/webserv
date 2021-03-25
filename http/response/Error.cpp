#include "Error.hpp"

Error::Error(ConfigServer* config, HttpRequest* req, std::string& route, std::string& location, BufferChain& writeChain, int statusCode) : HttpResponse(config, req, route, location)
{
	_statusCode = statusCode;

	int        		fd;
	std::string*	buff = NULL;

	_route = _config->getHTMLErrorPage(_statusCode);
	fd = open(_route.c_str(), O_RDONLY);
	if (fd != -1)
	{
		struct stat file;
		stat(_route.c_str(), &file);
		setContentType();
		setCharset();
		_contentLength = file.st_size;
		setServerName();
		setContentLocation();
		_streamReadFd = fd;
		_state.read = DONE; // TO DO is it ok ? same in File Download
		_state.readStream = READY;
	}
	else
	{
		buff = new std::string();
		buff->append("<!DOCTYPE html>\n<html>\n<body>\n\n<h1>");
		char *tmp = ft_itoa(_statusCode);
		buff->append(tmp).append(" ").append(_mapCodes.codes[_statusCode]);
		free(tmp);
		buff->append("</h1>\n\n</body>\n</html>\n");
		_contentType = "text/html";
		_charset = "utf-8";
		_contentLength = buff->length();
	}

	//appending the buffers
	writeChain.pushBack(getRawHeaders());
	if (buff)
		writeChain.pushBack(buff);
	std::cout << "/* message */ " << _mapCodes.codes[_statusCode] << std::endl;
}


void	Error::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
	(void) writeChain;

	HttpResponse::handleRead(readChain, writeChain);
	_streamWriteChain.flush();
}

// void	Error::handleWrite(BufferChain& readChain, BufferChain& writeChain)
// {
// 	(void) readChain;

// 	std::cout << "---------------------------------------------------------- " << writeChain <<std::endl;
// 	if (writeChain.getFirst() == NULL)
// 		throw HttpResponse::ConnectionClose(); 
// }

Error::~Error()
{

}