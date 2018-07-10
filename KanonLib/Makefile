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


${BIN_TARGET} : ${OBJ} #| BIN_DIR
	if [! -d $(DIR_BIN)]; then mkdir ${DIR_BIN}; fi;\
	${CC} -pthread -o $@ $^

# BIN_DIR:
# 	if [! -d $(DIR_BIN)]; then mkdir ${DIR_BIN}; fi;

${DIR_OBJ}/%.o : ${DIR_SRC}/%.cpp #| OBJ_DIR
	if [! -d $(DIR_OBJ)]; then mkdir ${DIR_OBJ}; fi; \
	${CC} ${CFLARGS} -o $@ -c $^

# OBJ_DIR:
# 	if [! -d $(DIR_OBJ)]; then mkdir ${DIR_OBJ}; fi;
	
	
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
