#include "Runnable.hpp"


Runnable::Runnable(int respawn, int detached) {
    _respawn = respawn;
	_detached = detached;
}

Runnable::~Runnable() {
    
}

void Runnable::run() {    
}
