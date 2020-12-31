#include <stdlib.h>
#include <iostream>
#include "utils.hpp"

void		*ft_calloc(size_t count, size_t size)
{
	char	*var;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	if (!(var = (char*)malloc(sizeof(char) * (count * size))))
		return (NULL);
	ft_bzero((void*)var, count * size);
	return ((void*)var);
}
