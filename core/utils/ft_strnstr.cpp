#include "utils.hpp"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	l;
	
	i = 0;
	l = ft_strlen(needle);
	while (len--)
	{
		if (*haystack == needle[i])
		{
			i++;
			if (needle[i] == '\0')
				return ((char *)haystack - l);
		}
		else
			i = 0;
		haystack++;
	}
	return (NULL);
}
