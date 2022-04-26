#CC = mpiicc
CC = mpicc

INCLUDES = 
LIBS = -lm

# Rules

.PHONY: all clean clean-all

all:	errexit collect_sendrecv collect_bcast collect_gather synch synch_s intsum bcast

errexit:	errexit.c
	$(CC) $(CFLAGS) $(INCLUDES) -o errexit.exe $^ $(LIBS)

collect_sendrecv:	collect_sendrecv.c
	$(CC) $(CFLAGS) $(INCLUDES) -o collect_sendrecv.exe $^ $(LIBS)

bcast:	bcast.c
	$(CC) $(CFLAGS) $(INCLUDES) -o bcast.exe $^ $(LIBS)

collect_bcast:	collect_bcast.c
	$(CC) $(CFLAGS) $(INCLUDES) -o collect_bcast.exe $^ $(LIBS)

collect_gather:	collect_gather.c
	$(CC) $(CFLAGS) $(INCLUDES) -o collect_gather.exe $^ $(LIBS)

synch:	synch.c
	$(CC) $(CFLAGS) $(INCLUDES) -o synch.exe $^ $(LIBS)

synch_s:	synch_s.c
	$(CC) $(CFLAGS) $(INCLUDES) -o synch_s.exe $^ $(LIBS)

intsum:	intsum.c
	$(CC) $(CFLAGS) $(INCLUDES) -o intsum.exe $^ $(LIBS)


clean:  
	rm -f *.o *.exe

clean-all: clean
	rm -f *~ core
