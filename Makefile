# Implicit variable definition
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11

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
			core/HttpConf.cpp \
			core/HttpServer.cpp

SRCS = $(CORESRC)


# Headers files

INCLUDES = -I core/ -I http/ 

# Object files

OBJECTSDIR = objs
OBJECTS = $(addprefix $(OBJECTDIR)/, $(subst .cpp,.o,$(SRCS)))

# Executable

NAME = webserv

# Implicit rules

$(OBJECTSDIR):
	mkdir -p $(OBJECTSDIR)

$(OBJECTSDIR)/%.o: ./%.c $(OBJECTSDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)


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

.PHONY: all clean fclean re