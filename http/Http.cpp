#include "Http.hpp"


Http::Http(Connection &c)
{
	_connection = c;
	_write_chain = c
}

Http::~Http()
{
}
