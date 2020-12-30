#include "utils.hpp"
#include <stdlib.h>

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
    free(src);
	return (dest);
}
