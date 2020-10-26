
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -std=c++11

SRCS = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(SRCS))

NAME = configParser

all:	$(NAME)

$(NAME):	$(OBJECTS)
			$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
		rm -f $(OBJECTS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re