#CC = mpiicc
CC = mpicc

INCLUDES = 
LIBS = 

.PHONY: all clean clean-all

all:	vector status deadlock no_deadlock pp

vector:		p2p_vector.c
	$(CC) $(CFLAGS) $(INCLUDES) -o p2p_vector.exe $< $(LIBS)

no_deadlock:	 p2p_no_deadlock.c
	$(CC) $(CFLAGS) $(INCLUDES) -o p2p_no_deadlock.exe $< $(LIBS)

deadlock:	 p2p_deadlock.c
	$(CC) $(CFLAGS) $(INCLUDES) -o p2p_deadlock.exe $< $(LIBS)

status:		p2p_status.c
	$(CC) $(CFLAGS) $(INCLUDES) -o p2p_status.exe $< $(LIBS)

pp:		pp_mpi.c
	$(CC) $(CFLAGS) $(INCLUDES) -o pp_mpi.exe $< $(LIBS)

clean:  
	rm -f *.o  *.exe

clean-all:	clean
	rm -f *~ core
