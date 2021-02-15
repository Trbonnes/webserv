#include "utils.hpp"
#include <stdlib.h>
#include <iostream>

char			*ft_realloc(char *src, size_t nb)
{
    char    *dest;
	size_t  i;

	if (!(dest = (char*)ft_calloc(nb, sizeof(char))))
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
    free(src);
	return (dest);
}
