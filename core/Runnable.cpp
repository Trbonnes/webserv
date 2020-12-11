#include "Runnable.hpp"

Runnable::Runnable() {
}

Runnable::Runnable(int respawn, int detached) {
    _respawn = respawn;
	_detached = detached;
}

Runnable::Runnable(const Runnable& r)
{
    _respawn = r._respawn;
    _detached = r._detached;
}

Runnable& Runnable::operator=(const Runnable& r)
{
    _respawn = r._respawn;
    _detached = r._detached;
    return *this;
}

Runnable::~Runnable() {
    
}

void Runnable::run() {    
}

bool Runnable::isRespawn()
{
    return _respawn;
}

bool Runnable::isDetached()
{
    return _detached;
}
