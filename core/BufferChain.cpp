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
	flush();
}

void	BufferChain::pushBack(std::string* toadd)
{
	_chain.push_back(toadd);
}

void	BufferChain::pushFront(std::string* toadd)
{
	_chain.push_front(toadd);
}

std::string* BufferChain::getFirst()
{
	if (_chain.empty())
		return NULL;
	return _chain.front();
}

std::string* BufferChain::getLast()
{
	if (_chain.empty())
		return NULL;
	return _chain.back();
}

void BufferChain::popFirst()
{
	_chain.pop_front();
}

BufferChain::iterator BufferChain::begin()
{
	return _chain.begin();
}

BufferChain::iterator BufferChain::end()
{
	return _chain.end();
}

size_t BufferChain::size()
{
	return _chain.size();
}

void	BufferChain::flush()
{
	std::string* tmp;
	while (_chain.size() > 0)
	{
		tmp = getFirst();
		delete tmp;
		popFirst();
	}
}

int		BufferChain::writeBufferToFd(BufferChain& chain, FD fd)
{
	int ret;
	std::string* buff;
	
	buff = chain.getFirst();
	ret = write(fd, buff->c_str(), buff->size());
	if (ret == -1) // TO DO check if write fails to write all bytes
		throw IOError();
	return ret;
}

int		BufferChain::readToBuffer(BufferChain& chain, FD fd)
{
	int ret;

	ret = read(fd, g_read_large, BUFFER_SIZE_LARGE); // TO DO keep this size ?
	if (ret == -1)
		throw IOError();
	if (ret > 0)
	{
		std::string* n = new std::string(g_read_large, ret);
		chain.pushBack(n);
	}
	return ret;
}

const char* IOError::what() const throw()
{
	return "An input or output eror has occurred";
}

std::ostream&	operator<<(std::ostream& out, BufferChain& chain)
{
	BufferChain::iterator it = chain.begin();

	out << "BufferChain: .size=" << chain.size() << std::endl;
	while (it != chain.end())
	{
		out << "|";
		out << **it;
		out << "| ";
		it++;
	}
	out << std::endl;
	return out;
}