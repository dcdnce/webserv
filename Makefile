SRCS=	srcs/server.cpp	\
		srcs/http/HTTPRequest.cpp \
		srcs/http/HTTPRequest_Parsing.cpp \
		srcs/http/HTTPTools.cpp \

OBJS=	$(SRCS:.cpp=.o)

CXX= c++

# Compiler flags (+ where to find headers)
CXXFLAGS= -Wall -Wextra -std=c++98
CXXFLAGS+= -Iincludes

# Linking flags(+ where to find libs)
LFLAGS=

NAME= webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $^ $(LFLAGS) -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re