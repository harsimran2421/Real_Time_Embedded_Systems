
CC=gcc

CFLAGS= -O0 -g $(CDEFS)

CFILES= pthread.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread

clean:
	-rm -f *.o *.d
	-rm -f pthread

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
