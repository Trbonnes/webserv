# Implicit variable definition
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -fsanitize=address # TO DO remove fsanitize of produciton

# Sources files

CORESRC = 	core/Runnable.cpp \
			core/webserver.cpp \
			core/HttpWorker.cpp \
			core/ProcessManager.cpp \
			core/ListenSocket.cpp \
			core/HttpConnection.cpp \
			core/HttpServer.cpp \
			core/ConfigServer.cpp \
			core/configFileParser.cpp \
			core/Location.cpp \
			core/Config.cpp

HTTPSRC = 	HTTP/HTTP.cpp \
			HTTP/httpRequestParser.cpp \
			HTTP/parseHeaders.cpp \
			HTTP/setHeaders.cpp \
			HTTP/Socket.cpp \
			HTTP/Methods/del.cpp \
			HTTP/Methods/get.cpp \
			HTTP/Methods/put.cpp \
			HTTP/utils/ft_atoi.cpp \
			HTTP/utils/ft_bzero.cpp \
			HTTP/utils/ft_itoa.cpp \
			HTTP/utils/ft_strcpy.cpp \
			HTTP/utils/ft_strdup.cpp \
			HTTP/utils/ft_strlen.cpp \
			HTTP/utils/get_next_line.cpp \
			HTTP/utils/ft_strcat.cpp \
			HTTP/utils/ft_calloc.cpp \
			HTTP/utils/ft_memcat.cpp \
			HTTP/CGI.cpp
			# HTTP/bin-cgi/cgi.cpp


SRCS = $(CORESRC) $(HTTPSRC)


# TO DO ADD HEADERS

# Headers files

INCLUDES = -I core/ -I HTTP/

# Object files

OBJECTSDIR = objs
OBJECTS = $(addprefix $(OBJECTSDIR)/, $(subst .cpp,.o,$(SRCS)))

$(OBJECTSDIR)/%.o: ./%.cpp
	mkdir -p $(OBJECTSDIR)/core $(OBJECTSDIR)/HTTP/bin-cgi $(OBJECTSDIR)/HTTP/Methods $(OBJECTSDIR)/HTTP/utils
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

# Executable

NAME = webserv

# Explicit rules

all:	$(NAME)


$(NAME):	$(OBJECTS)
			$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
		rm -f $(OBJECTS)
		rm -r $(OBJECTSDIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re $(OBJECTSDIR) $(NAME)
