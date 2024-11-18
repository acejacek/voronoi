CFLAGS = -Wall -Werror -Wextra -pedantic
LFLAGS = -lm

all: CFLAGS += -O2
all: exec

debug: CFLAGS += -DDEBUG -g
debug: exec

exec: main.c
	cc ${CFLAGS} -o voronoi main.c ${LFLAGS}
clean:
	rm -f *.o *.ppm voronoi
	rm -rf *.dSYM
