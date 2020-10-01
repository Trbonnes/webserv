#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

# define BUFFER_SIZE 1024

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strnjoin(char *s1, char *s2, int n)
{
	char	*s3;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !(s3 = (char*)malloc(sizeof(char) * (ft_strlen(s1) + n + 1))))
	{
		free(s1);
		return (NULL);
	}
	while (s1[++i])
		s3[i] = s1[i];
	while (j < n && s2[j])
	{
		s3[i + j] = s2[j];
		j++;
	}
	s3[i + j] = '\0';
	free(s1);
	return (s3);
}

static char		*ft_strcpy_gnl(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < BUFFER_SIZE + 1)
		dest[i++] = '\0';
	return (dest);
}

static int		ft_charset_gnl(char c, char *set)
{
	int		i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		if (set[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

static int		ft_nl(char **line, char *buf, int i)
{
	if (!(*line = ft_strnjoin(*line, buf, i)))
	{
		free(*line);
		*line = NULL;
		return (-1);
	}
	ft_strcpy_gnl(buf, buf + i + 1);
	return (1);
}

static int		ft_bnl(char **line, char *buf, int fd)
{
	int		ret;
	int		i;

	i = 0;
	if (!(*line = ft_strnjoin(*line, buf, BUFFER_SIZE)))
		return (-1);
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if ((i = ft_charset_gnl('\n', buf)) >= 0)
		{
			if (!(*line = ft_strnjoin(*line, buf, i)))
				return (-1);
			ft_strcpy_gnl(buf, buf + i + 1);
			return (1);
		}
		if (!(*line = ft_strnjoin(*line, buf, ret)))
			return (-1);
	}
	*buf = '\0';
	return (0);
}

int				get_next_line(int fd, char **line)
{
	char		tmp;
	int			i;
	int			ret;
	static char buf[BUFFER_SIZE + 1];

	if (fd < 0 || !line || read(fd, &tmp, 0) < 0)
		return (-1);
	if (!(*line = (char*)malloc(sizeof(char) * 1)))
		return (-1);
	**line = '\0';
	if ((i = ft_charset_gnl('\n', (char*)buf)) >= 0)
		return (ft_nl(line, buf, i));
	if ((ret = ft_bnl(line, buf, fd)) == -1)
		return (-1);
	return (ret);
}