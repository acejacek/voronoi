CFLAGS= -Wall -Werror -pedantic
LFLAGS=-lm

ALL: main.c
	cc ${CFLAGS} -o voronoi main.c ${LFLAGS}
