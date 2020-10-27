# Implicit variable definition
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++11

# Sources files

CORESRC = 	core/ConfigServer.cpp \
			core/Runnable.cpp \
			core/configFileParser.cpp \
			core/webserver.cpp \
			core/ProcessManager.cpp \
			core/HttpWorker.cpp \
			core/ListenSocket.cpp \
			core/Location.cpp \
			core/Config.cpp \
			core/HttpConnection.cpp \
			core/HttpServer.cpp

SRCS = $(CORESRC)


# TO DO ADD HEADERS

# Headers files

INCLUDES = -I core/ -I http/ 

# Object files

OBJECTSDIR = objs
OBJECTS = $(addprefix $(OBJECTSDIR)/, $(subst .cpp,.o,$(SRCS)))

$(OBJECTSDIR)/%.o: ./%.cpp
	mkdir -p $(OBJECTSDIR)/core $(OBJECTSDIR)/http
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

# Executable

NAME = webserv

# Explicit rules

all:	$(NAME)


$(NAME):	$(OBJECTS)
			$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
		rm -f $(OBJECTS)
		rm -r objs

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re $(OBJECTSDIR) $(NAME)
