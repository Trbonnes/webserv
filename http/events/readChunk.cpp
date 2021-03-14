#include "http/Http.hpp"

std::string*	Http::chunkify(char* buff, size_t len)
{
	//converting to hex
	std::stringstream ss;
	ss << std::hex << len;
	std::string* n = new std::string();
	std::string chunkstr = ss.str();

	// reserving the space needed
	n->reserve(chunkstr.size() + 2 + len + 2);

	// appending the chunk
	n->append(chunkstr);
	n->append("\r\n");
	if(len != 0)
	{
		// std::cout << "|" << strlen(buff) << "| " << start << " " << len << std::endl;
		n->append(buff, len);
	}
	n->append("\r\n");
	return n;
}

int		Http::readChunkToBuffer(BufferChain& chain, FD fd)
{
	int ret;
	std::string *n;

	ret = read(fd, g_read_large, BUFFER_SIZE_LARGE); // TO DO keep this size ?
	if (ret == -1)
		throw BufferChain::IOError();
	n = chunkify(g_read_large, ret);
	chain.pushBack(n);
	return ret;
}
