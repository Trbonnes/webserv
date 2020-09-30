CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror

SRCS = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(SRCS))

NAME = htppParser

all:	$(NAME)

$(NAME):	$(OBJECTS)
			$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
		rm -f $(OBJECTS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re