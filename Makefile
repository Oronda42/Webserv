SRCS = $(wildcard srcs/*.cpp)
INCLUDES = include/

OBJS = $(SRCS:.cpp=.o)

NAME = belloServ

CC = g++ -I$(INCLUDES) -Wall -Wextra -Werror -std=c++98 -g

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

SAME_LINE = \033[0G\033[2K

RESET = \033[0m

%.o: %.cpp
	@$(CC) -c $< -o $@
	@echo -n "$(SAME_LINE)$(AQUA)Compiling $(AQUA_BOLD)$< $(RESET)"

$(NAME):	$(OBJS)
	@echo
	@echo "$(PURPLE)Linking $(PURPLE)*.o into $(PURPLE_BOLD)$(NAME)$(RESET)"
	@$(CC) $(OBJS) -o $(NAME)
	@echo "$(GREEN_BOLD)Done compiling $(GREEN_UNDERLINE)$(NAME)"

all:		$(NAME)

clean:
	@rm -f $(OBJS)
	@echo "$(RED)Removing $(IRED)*.o$(RESET)"

fclean:		clean
	@rm -f $(NAME)
	@echo "$(RED)Removing $(IRED)$(NAME)$(RESET)"

re:			fclean all

.PHONY:	all clean fclean re

# CXX       = g++
# CXX_FLAGS = -g -std=c++98 


# SRC     = src
# INCLUDE = include

# EXECUTABLE  = belloServ


# all: $(EXECUTABLE)

# run: clean all
# 	clear
# 	./$(EXECUTABLE)

# $(EXECUTABLE): $(SRC)/*.cpp
# 	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

# clean:
# 	-rm $(EXECUTABLE)

# SRC = srcs/main.cpp srcs/MimeParser.cpp srcs/Request.cpp srcs/Response.cpp srcs/ConfigParser.cpp srcs/HttpCodesParser.cpp

# CC = g++
# CFLAGS =  -std=c++98 -Iinclude/#-fsanitize=address #-Wall -Werror -Wextra
# NAME = belloServ
# OBJS = ${SRC:.cpp=.o}
# INCLUDE = include

# ${NAME} : ${OBJS}
# 	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

# %.o:%.cpp
# 	$(CC) $(CFLAGS) -c $< -o $@
# clean :
# 	rm -rf ${OBJS}

# fclean : clean
# 	rm ${NAME}

# re : fclean ${NAME}

