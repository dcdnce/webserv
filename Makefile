################################################################################
# SOURCES                                                                      #
################################################################################

SRCS	:=	srcs/main.cpp \
			srcs/http/Client.cpp \
			srcs/http/ClientManager.cpp \
			srcs/http/Socket.cpp \
			srcs/http/Server.cpp \
			srcs/config/Config.cpp \
			srcs/config/Config_ParseServerBlock.cpp \
			srcs/config/Config_ParseLocationBlock.cpp \
			srcs/config/ServerBlock.cpp \
			srcs/config/ServerBlock_ParseDirectives.cpp \
			srcs/config/LocationBlock.cpp \
			srcs/config/LocationBlock_ParseDirectives.cpp \


OBJS	:=	$(SRCS:.cpp=.o)

################################################################################
# CONSTANTS                                                                    #
################################################################################

NAME		:=	webserv

CXX			:=	c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98

INCLUDES	:= -Iincludes
LIBS		:=

SHELL		:=	/bin/zsh

################################################################################
# VISUALS                                                                      #
################################################################################

# FOREGROUND
FG_BLACK	:=	\033[30m
FG_RED		:=	\033[31m
FG_GREEN	:=	\033[32m
FG_YELLOW	:=	\033[33m
FG_BLUE		:=	\033[34m
FG_MAGENTA	:=	\033[35m
FG_CYAN		:=	\033[36m
FG_WHITE	:=	\033[37m

# BACKGROUND
BG_BLACK	:=	\033[40m
BG_RED		:=	\033[41m
BG_GREEN	:=	\033[42m
BG_YELLOW	:=	\033[43m
BG_BLUE		:=	\033[44m
BG_MAGENTA	:=	\033[45m
BG_CYAN		:=	\033[46m
BG_WHITE	:=	\033[47m

# EFFECTS
RESET	:=	\033[0m
BOLD	:=	\033[1m
DIM		:=	\033[2m
UNDERL	:=	\033[4m
BLINK	:=	\033[5m
REVERSE	:=	\033[7m
HIDDEN	:=	\033[8m

# MANIPULATIONS
REWRITE	:=	\r\033[2K

# PRESETS with emojis
INFO	:=	$(BG_BLUE)$(FG_WHITE)$(BOLD) 📢
SUCCESS	:=	$(BG_GREEN)$(FG_WHITE)$(BOLD) ✔
ERROR	:=	$(BG_RED)$(FG_WHITE)$(BOLD) ✘

################################################################################
# TARGETS / RULES                                                              #
################################################################################

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo -n "$(REWRITE)$(INFO) Compiling $(notdir $<) $(RESET)"

all: $(NAME)

run: all
	@./$(NAME)

debug: CXXFLAGS += -g -fsanitize=address -DDEBUG
debug: re

# $^ = all dependencies
# $@ = target
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) $^ -o $@
	@echo "$(REWRITE)$(SUCCESS) $(NAME) compiled $(RESET)"

clean:
	@rm -f $(OBJS)
	@echo "$(ERROR) $(words $(OBJS)) files removed $(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(ERROR) $(NAME) removed $(RESET)"

re: fclean all

.PHONY: all run debug clean fclean re
