#include "FileUpload.hpp"

FileUpload::FileUpload(
	ConfigServer* config,
	HttpRequest* request,
	std::string route,
	std::string location) : HttpResponse(config, request, route, location)
{
	_location = location;

    // Getting path root and appending the file's name
    std::string filename = route.substr(route.find_last_of('/')); // TO DO very ugly
    _file = _config->getPutRoot().append(filename); // TO DO pas convaincu
    
    // opening the ouputStream
    // trying to open this existing file with O_TRUNCK to erase content while writing
    _streamWriteFd = open(_file.c_str(), O_WRONLY | O_TRUNC);
    std::cout << "00000000000000000 trying to creat file:" << _file <<std::endl;
    // if -1 then it doesn't exist
    _statusCode = NO_CONTENT;
    if (_streamWriteFd == -1)
    {
        std::cout << "FILE DOES NOT EXISTS or error" << std::endl;
        _statusCode = NO_CONTENT; // Should be 201 but the tester expect 204
        // Trying to create the file then
        _streamWriteFd = open(_file.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (_streamWriteFd == -1)
        {
            std::cout << "Failed to create" << _file.c_str() << std::endl;
            // TO DO throw error
        }
    }
    setContentLocation(); // TO DO probably not the right thing to do
    _state.writeStream = WAITING;
    // Waiting for the actual body to be written
	_state.write = WAITING;
}


void    FileUpload::handleRead(BufferChain& readChain, BufferChain& writeChain)
{
    (void) writeChain;

    HttpResponse::handleRead(readChain, writeChain);
    if (_state.read == DONE && _state.writeStream == DONE)
    {
        std::string *buff = getRawHeaders();
        writeChain.pushBack(buff);
        _state.writeStream = DONE;
        _state.write = READY;
    }
}

void    FileUpload::handleStreamWrite(BufferChain& readChain, BufferChain& writeChain)
{
    (void) readChain;

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
    std::cout << "ABORTING SADASDASDD" << std::endl;
    HttpResponse::abort();
    // don't really care if it fails
    unlink(_file.c_str());
}

FileUpload::~FileUpload()
{

}