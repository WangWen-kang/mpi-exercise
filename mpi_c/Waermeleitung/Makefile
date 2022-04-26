# compile and link program

CC = mpiicc
#CC = mpicc
# Debugging settings - comment out to disable
#CFLAGS += -g -DDEBUG
LIBS = -lm
INCLUDE =

# Files

OBJECTS_ser = waermeleitung.o initialisierung.o zeitschritt.o \
              ausgabe.o
OBJECTS_mpi = waermeleitung_mpi.o initialisierung_mpi.o zeitschritt.o \
              ausgabe.o randaustausch.o  dist_index.o

# Targets

.SUFFIXES = .c
.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<


all: 	waermeleitung waermeleitung_mpi

waermeleitung:	$(OBJECTS_ser)
	$(CC) -o $@.exe $^ $(LIBS)

waermeleitung_mpi: $(OBJECTS_mpi)
	$(CC) -o $@.exe $^ $(LIBS)

gnuplot:
	gnuplot ausgabe.gp > ausgabe.gif

clean:
	rm  -f *.o *.exe

clean-all: clean
	rm -f *~ ausgabe.gp ausgabe.gif ergebnis.gp core

.PHONY:	all clean clean-all
