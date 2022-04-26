#CC = mpiicc
CC = mpicc

LIBS = -lm
INCLUDE = 
.PHONY:	all clean clean-all
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

# Targets

all:	piapp piapp_mpi piprec piprec_worker

piapp:	piapp.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o piapp.exe $^ $(LIBS)

piapp_mpi:	piapp_mpi.c
	$(CC) $(CFLAGS) $(INCLUDE) -o piapp_mpi.exe $^ $(LIBS)

piprec:	piprec.c  numapp.c
	$(CC) $(CFLAGS) $(INCLUDE) -o piprec.exe $^ $(LIBS)

piprec_worker:	piprec_worker.c numapp.c
	$(CC) $(CFLAGS) $(INCLUDE) -o piprec_worker.exe $^ $(LIBS)

clean:
	rm  -f *.exe *.o

clean-all: clean
	rm -f *~ core

# Deps
