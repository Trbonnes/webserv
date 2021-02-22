#if !defined(BUFFER_CHAIN)
#define BUFFER_CHAIN

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include "utils/utils.hpp"

// Socket/Stream read write buffer
// default nginx client buffer size on 64 bit systems 
// It should be enough to hold most Http request headers, but we got to implement the case where it is too small
#define BUFFER_SIZE_MEDIUM 16384
// #define BUFFER_SIZE_LARGE 1024000
#define BUFFER_SIZE_LARGE 10 // TO DO don't forget to remove


// Buffer used in socket and filesystem read
extern char g_read_medium[BUFFER_SIZE_MEDIUM];
extern char g_read_large[BUFFER_SIZE_LARGE];

// This typedef makes fd manipulation a little bit more clear (thanks to @cchudant for this)
typedef int FD;

// Previously this class used to store the buffer in a struc with a char* and a size_t for the size
// But since the cstring library is prohibited it uses string for manipulation
class BufferChain
{
public:
	typedef std::list<std::string*>::iterator iterator;
private:

	// The actual buffer list
	std::list<std::string*> _chain;

public:

	// Coplien
	BufferChain();
	BufferChain(BufferChain&);
	BufferChain &operator=(BufferChain&);
	~BufferChain();

	// Adds a buffer to the list
	void	pushBack(std::string*);
	void	pushFront(std::string*);

	// return first buffer pair;
	std::string* getFirst();

	// return last buffer pair;
	std::string* getLast();
	
	// pop first buffer pair, doesn't free data
	void popFirst();

	// Destroy and free every buffer in the list
	void	flush();
	static int	writeBufferToFd(BufferChain&, FD);
	static int	readToBuffer(BufferChain&, FD);

	iterator begin();
	iterator end();
	size_t size();
};

class	IOError: public std::exception {
	public:
		virtual const char* what() const throw();
};


std::ostream&	operator<<(std::ostream&, BufferChain&);

// Non-class members to write and read directly to/from a BufferChain
#endif // BUFFER_CHAIN
