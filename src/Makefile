CC=clang++
CFLAGS= -c -std=c++11 -O2
# CFLAGS= -c -std=c++11 -g
LFLAGS=

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)
PROG=makechange

all: ${PROG}

${PROG}: ${OBJ}
	${CC} ${LFLAGS} -o $@ $^

${OBJ}: ${SRC}
	${CC} ${CFLAGS} $^

clean:
	rm -f *.o
	rm -f ${PROG}
