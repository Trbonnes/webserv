SRCS =	TEST/main.cpp 			\
		TEST/Socket.cpp 		\
		TEST/Config.cpp			\
		TEST/Location.cpp		\
		utils/get_next_line.cpp \
		Methods.cpp				\
		Methods/get.cpp			\
		Methods/head.cpp		\
		Methods/post.cpp		\
		Methods/put.cpp			\
		Methods/options.cpp		\
		Methods/patch.cpp		\
		Methods/trace.cpp		\
		Methods/delete.cpp		\
		Methods/connect.cpp

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
