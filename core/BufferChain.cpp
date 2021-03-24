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

BufferChain& BufferChain::operator=(BufferChain& chain)
{
	flush();
	_chain = chain._chain;
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

size_t BufferChain::totalSize()
{
	// TO DO might be optimizable by stroring a variable tha\s getting modified after each pushfront/pushback
	size_t total = 0;

	for (std::list<std::string*>::iterator it = _chain.begin(); it != _chain.end(); *it++)
	{
		total += (*it)->size();
	}
	return total;
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
	// std::cout << "About to write to fd " << fd << " with size " << buff->size() << std::endl;
	ret = write(fd, buff->c_str(), buff->size());
	// std::cout << "Written " << ret <<  std::endl;

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
		// std::cout << "STP  | " <<  *n << " |STP " << std::endl;
 		chain.pushBack(n);
	}
	return ret;
}

const char* BufferChain::IOError::what() const throw()
{
	return "An input or output error has occurred";
}

#include <algorithm>
std::ostream&	operator<<(std::ostream& out, BufferChain& chain)
{
	BufferChain::iterator it = chain.begin();

	out << "BufferChain: .size=" << chain.size() << std::endl;
	while (it != chain.end())
	{
		std::string toprint;
		if ((**it).size() > 1000)
			std::string toprint = (**it).size() > 10 ? std::string(**it, 0, 7).append("...") : **it;
		else
			toprint = **it;
		std::replace(toprint.begin(), toprint.end(), '\n', 'N');
		std::replace(toprint.begin(), toprint.end(), '\r', 'R');
		out << "|";
		out << toprint;
		out <<  "|" << "size: " << (**it).size() << " ";
		it++;
	}
	out << std::endl;
	return out;
}