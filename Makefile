SRCS =	TEST/main.cpp 			\
		TEST/Socket.cpp 		\
		TEST/Config.cpp			\
		utils/get_next_line.cpp \
		Methods.cpp				\
		get.cpp

OBJS = $(SRCS:.cpp=.o)
NAME = methods
RM = rm -f
CC = clang++
CFLAGS = -Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
