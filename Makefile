CFLAGS = -Wall -Werror -pedantic -O2
LFLAGS = -lm

ALL: main.c
	cc ${CFLAGS} -o voronoi main.c ${LFLAGS}

clean:
	rm -f *.o *.ppm voronoi
