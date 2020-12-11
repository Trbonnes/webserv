#include "utils.hpp"

char			*ft_strcat(char *dest, const char *src)
{
	int		i;
	int		len_dest;

	i = 0;
	len_dest = (int)ft_strlen((const char*)dest);
	while (src[i])
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = src[i];
	return (dest);
}
