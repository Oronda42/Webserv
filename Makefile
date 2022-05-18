CXX       = clang++
CXX_FLAGS = -std=c++98

BIN     = bin
SRC     = src
INCLUDE = include

EXECUTABLE  = belloServ


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm $(BIN)/*

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

