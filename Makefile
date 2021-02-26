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
			core/webserver.cpp \
			core/utils/ft_atoi.cpp \
			core/utils/ft_bzero.cpp \
			core/utils/ft_itoa.cpp \
			core/utils/ft_strcpy.cpp \
			core/utils/ft_strdup.cpp \
			core/utils/ft_strlen.cpp \
			core/utils/get_next_line.cpp \
			core/utils/ft_strcat.cpp \
			core/utils/ft_calloc.cpp \
			core/utils/ft_memcat.cpp \
			core/utils/ft_memcpy.cpp \
			core/utils/ft_strnstr.cpp \
			core/utils/ft_realloc.cpp

HTTPSRC =	http/Http.cpp \
			http/HttpResponse.cpp \
			http/HttpRequest.cpp \
			http/httpRequestParser.cpp \
			http/responseHandlers.cpp \
			http/parseHeaders.cpp \
			http/setHeaders.cpp \
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
			core/utils/utils.hpp \
			http/statusCodes.hpp \
			http/Http.hpp \
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
				$(OBJECTSDIR)/core/utils \
				$(OBJECTSDIR)/http/bin-cgi \
				$(OBJECTSDIR)/http/Methods


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
