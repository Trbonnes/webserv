SRCS =	main.cpp 						\
		HTTP/TEST/ConfigServer.cpp		\
		HTTP/TEST/Location.cpp			\
		Socket/httpRequestParser.cpp	\
		Socket/parseHeaders.cpp			\
		Socket/Socket.cpp				\
		HTTP/utils/get_next_line.cpp 	\
		HTTP/utils/ft_strcpy.cpp		\
		HTTP/utils/ft_strlen.cpp		\
		HTTP/utils/ft_strdup.cpp		\
		HTTP/utils/ft_bzero.cpp			\
		HTTP/utils/ft_atoi.cpp			\
		HTTP/utils/ft_itoa.cpp 			\
		HTTP/HTTP.cpp					\
		HTTP/CGI.cpp					\
		HTTP/Methods/get.cpp			\
		HTTP/Methods/put.cpp 			\
		HTTP/Methods/del.cpp			\
		HTTP/setHeaders.cpp


OBJS = $(SRCS:.cpp=.o)
NAME = Server
RM = rm -f
CC = clang++
CFLAGS = -Wall -Wextra -Werror  -fsanitize=address -g3 #-lcrypt

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
