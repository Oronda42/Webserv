SRC = server.cpp
CC = clang++
CFLAGS =  -std=c++98 -fsanitize=address #-Wall -Werror -Wextra
NAME = belloServ
OBJS = ${SRC:.cpp=.o}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	rm -rf ${OBJS}

fclean : clean
	rm ${NAME}

re : fclean ${NAME}