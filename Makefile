SRCS = $(wildcard srcs/*.cpp)
INCLUDES = includes/

OBJS = $(SRCS:.cpp=.o)

NAME = belloServ

CC = g++

CFLAGS = -I$(INCLUDES) -Wall -Wextra -Werror -std=c++98
ifdef DEBUG
	CFLAGS += -g -DDEBUG=true
endif

SHELL = zsh

AQUA = \033[0;96m
AQUA_BOLD = \033[1;96m

PURPLE = \033[0;95m
PURPLE_BOLD = \033[1;95m

GREEN = \033[0;92m
GREEN_BOLD = \033[1;92m
GREEN_UNDRLINE = \033[4;32m

RED = \033[0;91m
IRED = \033[0;31m
RED_BOLD = \033[1;91m

YELLOW = \033[0;33m

SAME_LINE = \033[0G\033[2K

RESET = \033[0m

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -n "$(SAME_LINE)$(AQUA)Compiling $(AQUA_BOLD)$< $(RESET)"

$(NAME):	$(OBJS)
	@echo
	@echo "$(PURPLE)Linking *.o into $(PURPLE_BOLD)$(NAME)$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN_BOLD)Done compiling $(GREEN_UNDERLINE)$(NAME)"

all: $(NAME)

bonus: all

debug: fclean
	@echo "$(YELLOW)Enabling debug mode$(RESET)"
	@$(MAKE) DEBUG=1 --no-print-directory


clean:
	@rm -f $(OBJS)
	@echo "$(RED)Removing $(IRED)*.o$(RESET)"

fclean:		clean
	@rm -f $(NAME)
	@echo "$(RED)Removing $(IRED)$(NAME)$(RESET)"

re:			fclean all

.PHONY:	all clean fclean re bonus