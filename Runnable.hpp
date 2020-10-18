#if !defined(RUNNABLE)
#define RUNNABLE


class Runnable
{
public:
	Runnable(int respawn, int detached);
	Runnable(Runnable &&) = default;
	Runnable(const Runnable &) = default;
	Runnable &operator=(Runnable &&) = default;
	Runnable &operator=(const Runnable &) = default;
	virtual ~Runnable();
	virtual void	run();
	bool	_respawn;
	bool	_detached;
	int		_status;
};
#endif // RUNNABLE
