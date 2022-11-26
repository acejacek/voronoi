CFLAGS= -Wall -Werror -pedantic

ALL: main.c
	cc ${CFLAGS} -o voronoi main.c
