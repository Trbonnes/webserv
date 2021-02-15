#if !defined(BUFFER_CHAIN)
#define BUFFER_CHAIN

#include <sys/types.h>
#include <unistd.h>
#include <queue>
#include <cstring> 

// Socket/Stream read write buffer
// default nginx client buffer size on 64 bit systems 
// It should be enough to hold most Http request headers, but we got to implement the case where it is too small
#define BUFFER_SIZE_MEDIUM 16384
#define BUFFER_SIZE_LARGE 1024000


// Buffer used in socket and filesystem read
extern char g_read_medium[BUFFER_SIZE_MEDIUM];
extern char g_read_large[BUFFER_SIZE_LARGE];

// This typedef makes fd manipulation a little bit more clear (thanks to @cchudant for this)
typedef int FD;
// This class holds a list of char buffers
// They can vary in size
class BufferChain
{

private:
	// The buffer pair structure
	typedef struct	buffer_s
	{
		char *data;
		size_t size;
	}				buffer_t;

	// The actual buffer list
	std::queue<buffer_t> _chain;

public:

	// Cooplien
	BufferChain();
	BufferChain(BufferChain&);
	BufferChain &operator=(BufferChain&);
	~BufferChain();

	// Adds a buffer to the list
	void	pushBack(char *, size_t);
	// Copy into a new buffer that is added to the list
	void	copyPushBack(char *, size_t);

	// return first buffer pair;
	buffer_t& getFirst();
	
	// pop and returns first buffer pair;
	buffer_t popFirst();

	// Destroy and free every buffer in the list
	void	flush();
	static int	writeBufferToFd(BufferChain&, FD);
	static int	readToBuffer(BufferChain&, FD, size_t);
};

class	IOError: public std::exception {
	public:
		virtual const char* what() const throw();
};

// Non-class members to write and read directly to/from a BufferChain
#endif // BUFFER_CHAIN
