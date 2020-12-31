#include "utils.hpp"
#include <iostream>

char			*ft_memcat(char *dest, const char *src, size_t nb)
{
	size_t  i;
	int		len_dest;

	i = 0;
	len_dest = (int)ft_strlen((const char*)dest);
	while (src[i])
	{
		std::cerr << "mem: " << nb << std::endl;
		std::cerr << "len_dest: " << len_dest << std::endl;
		std::cerr << "i + len_dest: " << i + len_dest << std::endl;
		dest[len_dest + i] = src[i];
		std::cerr << "OK" << std::endl;
		i++;
	}
	dest[len_dest + i] = '\0';
	return (dest);
}
