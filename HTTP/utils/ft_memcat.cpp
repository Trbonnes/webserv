#include "utils.hpp"

char			*ft_memcat(char *dest, const char *src, size_t nb)
{
	size_t  i;
	int		len_dest;

	i = 0;
	len_dest = (int)ft_strlen((const char*)dest);
	while (i < nb)
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = src[i];
	return (dest);
}
