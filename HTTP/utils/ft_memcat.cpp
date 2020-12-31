#include "utils.hpp"
#include <iostream>

char			*ft_memcat(char *dest, const char *src, size_t nb)
{
	size_t  i;
	int		len_dest;

	i = 0;
	(void)nb;
	len_dest = (int)ft_strlen((const char*)dest);
	while (src[i])
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (dest);
}
