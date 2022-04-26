#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ritz.h"

/*
 +-------------+----------------+------------------------------------+
 |  parameter  |  input/output  |  description                       |
 +-------------+----------------+------------------------------------+
 |  n          |  input         |  global vectorlength               |
 |  y          |  input         |  local part of global vector       |
 |  x          |  output        |  reconstructed global vector       |
 +-------------+----------------+------------------------------------+
 |                                                                   |
 |  Purpose of this subroutine is to collect the local parts of a    |
 |  vector from all processor and combine them to the global vector. |
 |                                                                   |
 +-------------------------------------------------------------------+
*/


void global_vector( int n, double *y, double *x ){

   int nproc, myid;
   int firstind[NPROCMAX], i, ip, ia, nl;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   dist_index( n, firstind );
   nl = firstind[myid+1] - firstind[myid];
   ia = firstind[myid];
   for (i=0;i<nl;i++){
     x[ia+i] = y[i];
   }

   for (ip = 0 ; ip < nproc ;ip++){
      nl = firstind[ip+1] - firstind[ip];
      ia = firstind[ip];
      MPI_Bcast( &x[ia], nl , MPI_DOUBLE, ip, MPI_COMM_WORLD );
   }
} 
