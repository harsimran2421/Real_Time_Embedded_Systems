
CC=gcc

CFLAGS= -O3 -g 
LIBS= -lpthread -lrt

PRODUCT=feasibility_tests_EDF

HFILES=

CFILES= feasibility_tests.c \ EDF_Functions.c

OBJS= feasibility_tests.o EDF_Functions.o

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~ *.d
	-rm -f ${PRODUCT} ${GARBAGE}

feasibility_tests_EDF:	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

depend:

%.o : %.c 
	$(CC) -MD $(CFLAGS) -c $<
