INCLUDE_DIRS =
LIB_DIRS =
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS=

HFILES=
CFILES= timed_mutex.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:timed_mutex

clean:
	-rm -f *.o *.d
	-rm -f timed_mutex

timed_mutex: timed_mutex.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lpthread -lrt

.c.o:
	$(CC) $(CFLAGS) -c $<
