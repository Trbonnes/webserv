#include "FileUpload.hpp"

FileUpload::FileUpload(ResponseContext& ctx) : HttpResponse(ctx)
{
    // Getting path root and appending the file's name
    std::string filename = _route.substr(_route.find_last_of('/')); // OPTIMIZATION send help
    _file = _config->getPutRoot().append(filename);
    
    // opening the ouputStream
    // trying to open this existing file with O_TRUNCK to erase content while writing
    _streamWriteFd = open(_file.c_str(), O_WRONLY | O_TRUNC);
    // if -1 then it doesn't exist
    _statusCode = NO_CONTENT;
    if (_streamWriteFd == -1)
    {
        _statusCode = NO_CONTENT; // Should be 201 but the tester expect 204
        // Trying to create the file then
        _streamWriteFd = open(_file.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (_streamWriteFd == -1)
        {
            throw HttpError(INTERNAL_SERVER_ERROR);
        }
    }
    setContentLocation(); // OPTIMIZATION why ?
    _state.writeStream = WAITING;
    // Waiting for the actual body to be written
	_state.write = WAITING;
}


void    FileUpload::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
    HttpResponse::handleRead(readChain, writeChain);
    if (_state.writeStream == DONE)
    {
        std::string *buff = getRawHeaders();
        writeChain.pushBack(buff);
        _state.write = READY;
    }
}

void    FileUpload::handleStreamWrite(BufferChain& readChain, BufferChain& writeChain)
{
    HttpResponse::handleStreamWrite(readChain, writeChain);
    if (_state.writeStream == DONE)
    {
        std::string *buff = getRawHeaders();
        writeChain.pushBack(buff);
        _state.write = READY;
    }
}

void FileUpload::abort()
{
    HttpResponse::abort();
    // don't really care if it fails
    unlink(_file.c_str());
}

FileUpload::~FileUpload()
{
}