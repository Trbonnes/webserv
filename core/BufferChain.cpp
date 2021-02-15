#include "BufferChain.hpp"

char g_read_medium[BUFFER_SIZE_MEDIUM] = {};
char g_read_large[BUFFER_SIZE_LARGE] = {};

BufferChain::BufferChain()
{
}

BufferChain::BufferChain(BufferChain&)
{
	// TO DO add deep copy
}

BufferChain& BufferChain::operator=(BufferChain&)
{
	// TO DO add flush then deep copy
	return (*this);
}

BufferChain::~BufferChain()
{
}

void	BufferChain::pushBack(char* toadd, size_t size)
{
	if (size == 0)
		return;
	buffer_t pair;
	pair.data = toadd;
	pair.size = size;
	_chain.push(pair);
}

void	BufferChain::copyPushBack(char *tocopy, size_t size)
{
	if (size == 0)
		return;
	char* n = new char[size]();
	std::memcpy(n, tocopy, std::min(size, (size_t)BUFFER_SIZE_LARGE));
	pushBack(n, size);
}

BufferChain::buffer_t& BufferChain::getFirst()
{
	return _chain.front();
}

BufferChain::buffer_t BufferChain::popFirst()
{
	buffer_t pair = _chain.front();
	_chain.pop();
	return pair;
}

void	BufferChain::flush()
{
	// TO DO deep destruction
}

int		BufferChain::writeBufferToFd(BufferChain& chain, FD fd)
{
	int ret;
	BufferChain::buffer_t buff;
	
	buff = chain.popFirst();
	ret = write(fd, buff.data, buff.size);
	if (ret == -1)
		throw IOError();
	return ret;
}

int		BufferChain::readToBuffer(BufferChain& chain, FD fd)
{
	int ret;
	
	ret = read(fd, g_read_large, BUFFER_SIZE_LARGE); // TO DO keep this size ?
	if (ret == -1)
		throw IOError();
	chain.copyPushBack(g_read_large, ret);
	return ret;
}

const char* IOError::what() const throw()
{
	return "An input or output eror has occurred";
}
