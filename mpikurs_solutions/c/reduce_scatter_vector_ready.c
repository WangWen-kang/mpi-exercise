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
  |  y          |  input         |  partial sum of global vector      |
  |  x          |  output        |  local elements of full vector     |
  +-------------+----------------+------------------------------------+
  |                                                                   |
  |  Purpose of this subroutine is to collect partial result vectors  |
  |  from all processor and add them up to the global result.         |
  |                                                                   |
 +-------------------------------------------------------------------+
*/

void reduce_vector( int n, double *y, double *x ){

   int nproc, myid;
   int firstind[NPROCMAX], recvcounts[NPROCMAX], ip, nl, ia;
   MPI_Status stat;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   dist_index( n, firstind );
   for (ip = 0 ; ip < nproc ;ip++){
      recvcounts[ip] = firstind[ip+1] - firstind[ip];

   }
   MPI_Reduce_scatter(&y[0],&x[0],recvcounts,MPI_DOUBLE,MPI_SUM, MPI_COMM_WORLD);
}
