# Implicit variable definition
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# Sources files

CORESRC = 	core/Runnable.cpp \
			core/HttpWorker.cpp \
			core/ProcessManager.cpp \
			core/ListenSocket.cpp \
			core/HttpServer.cpp \
			core/Log.cpp \
			core/ConfigServer.cpp \
			core/configFileParser.cpp \
			core/Location.cpp \
			core/Config.cpp \
			core/Connection.cpp \
			core/Signals.cpp \
			core/BufferChain.cpp \
			core/webserver.cpp

HTTPSRC =	HTTP/HTTP.cpp \
			HTTP/httpRequestParser.cpp \
			HTTP/parseHeaders.cpp \
			HTTP/setHeaders.cpp \
			HTTP/HttpRequest.cpp \
			HTTP/LoadController.cpp \
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
			HTTP/utils/ft_realloc.cpp \
			HTTP/CGI.cpp


HEADERS = 	core/Location.hpp \
			core/Config.hpp \
			core/Runnable.hpp \
			core/Log.hpp \
			core/ProcessManager.hpp \
			core/Compare.hpp \
			core/HttpWorker.hpp \
			core/ListenSocket.hpp \
			core/ConfigServer.hpp \
			core/HttpServer.hpp \
			HTTP/utils/utils.hpp \
			HTTP/statusCodes.hpp \
			HTTP/LoadController.hpp \
			HTTP/HTTP.hpp \
			HTTP/HttpRequest.hpp \
			core/BufferChain.hpp \
			HTTP/CGI.hpp \
			core/Connection.hpp


INCLUDES = -I .

SRCS = $(CORESRC) $(HTTPSRC)

NAME = webserv

# Object files

OBJECTSDIR = objs
OBJECTS = $(addprefix $(OBJECTSDIR)/, $(subst .cpp,.o,$(SRCS)))

OBJECTSDIRS = 	$(OBJECTSDIR)/core \
				$(OBJECTSDIR)/HTTP/bin-cgi \
				$(OBJECTSDIR)/HTTP/Methods \
				$(OBJECTSDIR)/HTTP/utils


$(OBJECTSDIR)/%.o: ./%.cpp $(HEADERS)
	@mkdir -p $(OBJECTSDIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

all:	$(NAME)

$(NAME):	$(HEADERS) $(OBJECTS) | $(OBJECTSDIRS)
			$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

clean:
		rm -f $(OBJECTS)
		rm -rf $(OBJECTSDIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re $(OBJECTSDIR)
