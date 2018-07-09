DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin
DIR_INC = ./src

TARGET = kanon
BIN_TARGET = $(DIR_BIN)/$(TARGET)

SRC = $(wildcard $(DIR_SRC)/*.cpp)
# INCLUDE = $(wildcard ${DIR_INC}/*.h)
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CC = g++
CFLARGS = -g -Wall -I$(DIR_INC)


${BIN_TARGET} : ${OBJ} | BIN_DIR
	${CC} -pthread -o $@ $^

BIN_DIR:
	mkdir ${DIR_BIN}

${DIR_OBJ}/%.o : ${DIR_SRC}/%.cpp | OBJ_DIR
	${CC} ${CFLARGS} -o $@ -c $<

OBJ_DIR:
	mkdir ${DIR_OBJ}
	
	
# ./bin/kanon : main.o ThreadPool.o
# 	$(CC) -o ./bin/kanon main.o ThreadPool.o
# main.o : main.cpp
# 	$(CC) $(CFLARGS) -g -c main.cpp
# ThreadPool.o : ./threadLib/ThreadPool.h ./threadLib/ThreadPool.cpp
	# $(CC) $(CFLARGS) -c ./threadLib/ThreadPool.cpp

.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*.o
	rm  -rf ${DIR_BIN}/*
