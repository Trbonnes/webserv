SRCS =	TEST/main.cpp 			\
		TEST/Socket.cpp 		\
		TEST/ConfigServer.cpp	\
		TEST/Location.cpp		\
		utils/get_next_line.cpp \
		utils/ft_strcpy.cpp		\
		utils/ft_strlen.cpp		\
		utils/ft_strdup.cpp		\
		utils/ft_bzero.cpp		\
		HTTP.cpp				\
		CGI.cpp					\
		Methods/get.cpp			\
		Methods/head.cpp		\
		setHeaders.cpp

OBJS = $(SRCS:.cpp=.o)
NAME = methods
RM = rm -f
CC = clang++
CFLAGS = -Wall -Wextra -Werror # -fsanitize=address -g3 #-lcrypt

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
