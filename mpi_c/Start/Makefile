#CC = mpiicc
CC = mpicc
INCLUDES = 
LIBS = -lm

.PHONY: all clean clean-all

all:	 hello_mpi zeit_mpi


hello_mpi: hello_mpi.c
	$(CC) $(CFLAGS) $(INCLUDES) -o hello_mpi.exe $< $(LIBS)

zeit_mpi: zeit_mpi.c
	$(CC) $(CFLAGS) $(INCLUDES) -o zeit_mpi.exe $< $(LIBS)

prime_mpi: prime_mpi.c
	$(CC) $(CFLAGS) $(INCLUDES) -o prime_mpi.exe $< -lm

clean:
	rm -f *.o  *.exe

clean-all: clean
	rm -f *~ core
