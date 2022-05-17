SRC = srcs/main.cpp srcs/MimeParser.cpp srcs/Request.cpp srcs/Response.cpp srcs/ConfigParser.cpp

CC = clang++
CFLAGS =  -std=c++98 -Iinclude/#-fsanitize=address #-Wall -Werror -Wextra
NAME = belloServ
OBJS = ${SRC:.cpp=.o}
INCLUDE = /includes

${NAME} : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	rm -rf ${OBJS}

fclean : clean
	rm ${NAME}

re : fclean ${NAME}