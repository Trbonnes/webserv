#if !defined(RUNNABLE)
#define RUNNABLE


class Runnable
{
private:
	bool	_respawn;
	bool	_detached;
	// int		_status;

public:
	Runnable();
	Runnable(const Runnable &);
	Runnable(int respawn, int detached);
	Runnable &operator=(const Runnable &);
	virtual ~Runnable();
	virtual void	run();
	bool isRespawn();
	bool isDetached();
};
#endif // RUNNABLE
