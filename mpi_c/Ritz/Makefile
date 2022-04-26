# compile and link program

CC = mpiicc -O3
#CC = mpicc
LIBS = -lm
INCLUDE = 

OBJECTS_ritz = ritz.o mv.o 
OBJECTS_col = ritz_dist_col.o mv.o dist_index.o dist_matrix_colblock.o reduce_vector.o
OBJECTS_row = ritz_dist_row.o mv.o dist_index.o dist_matrix_rowblock.o global_vector.o

.SUFFIXES: .c
.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

.PHONY:	all clean clean-all

# Targets

all:	ritz ritz_dist_col ritz_dist_row

ritz:	$(OBJECTS_ritz)
	$(CC) $(CFLAGS) -o ritz.exe $^ $(LIBS)

ritz_dist_col:	$(OBJECTS_col)
	$(CC) $(CFLAGS) -o ritz_dist_col.exe $^ $(LIBS)

ritz_dist_row:	$(OBJECTS_row)
	$(CC) $(CFLAGS) -o ritz_dist_row.exe $^ $(LIBS)

clean:
	rm  -f $(OBJECTS_ritz) $(OBJECTS_col) $(OBJECTS_row) *.exe

clean-all: clean
	rm -f *~ core

