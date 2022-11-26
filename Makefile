CFLAGS= -Wall -Werror -pedantic -ggdb

ALL: main.c
	cc ${CFLAGS} -o voronoi main.c
