#ifndef UTILS_HPP
# define UTILS_HPP

int     get_next_line(int fd, char **line);
char	*ft_strcpy(char *dest, const char *src);
int     ft_strlen(const char *str);
char	*ft_strdup(const char *s);
void	ft_bzero(void *s, size_t n);
int		ft_atoi(const char *s);
char	*ft_itoa(int n);

#endif
