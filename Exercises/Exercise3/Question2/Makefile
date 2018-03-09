INCLUDE_DIRS =
LIB_DIRS =
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS=

HFILES=
CFILES= mutexes.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:mutex

clean:
	-rm -f *.o *.d
	-rm -f mutex

mutex: mutexes.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lpthread -lrt

.c.o:
	$(CC) $(CFLAGS) -c $<
