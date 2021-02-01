#if !defined(RUNNABLE)
#define RUNNABLE

#include <exception>

class Runnable
{
private:
	bool	_respawn;
	bool	_detached;
	// int		_status;

public:
	class RunnableLaunchException: public std::exception
	{
		public:
			const char * what () const throw ()
			{
				return "Runnable failed to launch";
			}
	};
	Runnable();
	Runnable(const Runnable &);
	Runnable(int respawn, int detached);
	Runnable &operator=(const Runnable &);
	virtual ~Runnable();
	virtual void run();
	bool isRespawn();
	bool isDetached();
	virtual Runnable* clone() const = 0;
};
#endif // RUNNABLE
