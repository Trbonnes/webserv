#if !defined(RUNNABLE)
#define RUNNABLE


class Runnable
{
private:
	Runnable(const Runnable &);
	Runnable &operator=(const Runnable &);

public:
	Runnable(int respawn, int detached);
	virtual ~Runnable();
	virtual void	run();
	bool	_respawn;
	bool	_detached;
	int		_status;
};
#endif // RUNNABLE
