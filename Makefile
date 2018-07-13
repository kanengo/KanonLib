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
CFLARGS = -std=c++11 -g -Wall -I$(DIR_INC)


${BIN_TARGET} : ${OBJ} | BIN_DIR
	${CC} -pthread $^ -o $@

${DIR_OBJ}/%.o : ${DIR_SRC}/%.cpp | OBJ_DIR
	${CC} ${CFLARGS} -fPIC  -o $@ -c $<

BIN_DIR:
	@if [ ! -d $(DIR_BIN) ]; then mkdir ${DIR_BIN}; fi;


OBJ_DIR:
	@if [ ! -d $(DIR_OBJ) ]; then mkdir ${DIR_OBJ}; fi;

#make libso
# libkanon.so : ${OBJ}
	# ${CC} -fPIC -shared -pthread -o $@ $^

# ${DIR_OBJ}/%.o : ${DIR_SRC}/%.cpp
# 	${CC} ${CFLARGS} -fPIC  -o $@ -c $<

	

.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*.o
	rm  -rf ${DIR_BIN}/*