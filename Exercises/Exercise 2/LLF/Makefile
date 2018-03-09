
CC=gcc

CFLAGS= -O3 -g 
LIBS= -lpthread -lrt

PRODUCT=feasibility_tests_LLF

HFILES=

CFILES= LLF_feasibility.c \ LLF_Functions.c

OBJS= LLF_feasibility.o LLF_Functions.o

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~ *.d
	-rm -f ${PRODUCT} ${GARBAGE}

feasibility_tests_LLF:	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

depend:

%.o : %.c 
	$(CC) -MD $(CFLAGS) -c $<
