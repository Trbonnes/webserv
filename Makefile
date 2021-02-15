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

HTTPSRC =	http/HttpResponse.cpp \
			http/httpRequestParser.cpp \
			http/parseHeaders.cpp \
			http/setHeaders.cpp \
			http/HttpRequest.cpp \
			http/LoadController.cpp \
			http/Methods/del.cpp \
			http/Methods/get.cpp \
			http/Methods/put.cpp \
			http/utils/ft_atoi.cpp \
			http/utils/ft_bzero.cpp \
			http/utils/ft_itoa.cpp \
			http/utils/ft_strcpy.cpp \
			http/utils/ft_strdup.cpp \
			http/utils/ft_strlen.cpp \
			http/utils/get_next_line.cpp \
			http/utils/ft_strcat.cpp \
			http/utils/ft_calloc.cpp \
			http/utils/ft_memcat.cpp \
			http/utils/ft_realloc.cpp \
			http/CGI.cpp


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
			http/utils/utils.hpp \
			http/statusCodes.hpp \
			http/LoadController.hpp \
			http/HttpResponse.hpp \
			http/HttpRequest.hpp \
			core/BufferChain.hpp \
			http/CGI.hpp \
			core/Connection.hpp


INCLUDES = -I .

SRCS = $(CORESRC) $(HTTPSRC)

NAME = webserv

# Object files

OBJECTSDIR = objs
OBJECTS = $(addprefix $(OBJECTSDIR)/, $(subst .cpp,.o,$(SRCS)))

OBJECTSDIRS = 	$(OBJECTSDIR)/core \
				$(OBJECTSDIR)/http/bin-cgi \
				$(OBJECTSDIR)/http/Methods \
				$(OBJECTSDIR)/http/utils


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
