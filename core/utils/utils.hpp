#ifndef UTILS_HPP
# define UTILS_HPP

# include <string.h>
# include <stdlib.h>

int     get_next_line(int fd, char **line);
char	*ft_strcpy(char *dest, const char *src);
int     ft_strlen(const char *str);
char	*ft_strdup(const char *s);
void	ft_bzero(void *s, size_t n);
int		ft_atoi(const char *s);
char	*ft_itoa(int n);
char    *ft_strcat(char *dest, const char *src);
void    *ft_calloc(size_t count, size_t size);
char    *ft_memcat(char *dest, const char *src, size_t nb);
char	*ft_realloc(char *src, size_t nb);
void 	*ft_memcpy(void * dst, void const * src, size_t len);
#endif
