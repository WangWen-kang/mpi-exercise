CC = mpiicc

#Debug settings
#CFLAGS += -g

INCLUDES = 
INCLUDE_MKL = -I$(MKLROOT)/include
LIBS = 
LIB_MKL= $(LIBS) -L$(MKLROOT)/lib/intel64  -Wl,--start-group -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -Wl,--end-group -lpthread
# load mkl-module with:   module add intel-parallel-studio 
.PHONY: all clean clean-all


all:	time_dgemv_O0 time_dgemv_O3  time_dgemv_mkl

time_dgemv_O0: 
	$(CC)  -O0 -o $@.exe time_dgemv.c dgemv.c

time_dgemv_O3: 
	$(CC) -O3 -o $@.exe time_dgemv.c dgemv.c

time_dgemv_mkl: 
	$(CC) -O3  -DUSE_MKL $(INCLUDE_MKL) -o $@.exe time_dgemv.c $(LIB_MKL)

clean:  
	rm -f *.o  *.exe

clean-all: clean
	rm -f *~ core
