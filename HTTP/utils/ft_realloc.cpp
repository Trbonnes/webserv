#include "utils.hpp"
#include <stdlib.h>

char			*ft_realloc(char *src, size_t nb)
{
    char    *dest;
	size_t  i;

	dest = (char*)ft_calloc(nb, sizeof(char));
	i = 0;
	while (i < nb)
	{
		dest[i] = src[i];
		i++;
	}
    free(src);
	return (dest);
}
