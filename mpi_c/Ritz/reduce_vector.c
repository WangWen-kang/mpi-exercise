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
   int firstind[NPROCMAX], ip, displ, count;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   dist_index( n, firstind );
   MPI_Reduce(y,x,n,MPI_DOUBLE,MPI_SUM,0, MPI_COMM_WORLD);
   if (myid==0) {      
      for (ip = 1 ; ip < nproc ;ip++){
         displ = firstind[ip]; count =  firstind[ip+1] -  firstind[ip];
         MPI_Send(&x[displ],count,MPI_DOUBLE,ip,0, MPI_COMM_WORLD);
      }
   }
   else {
      count =  firstind[myid+1] -  firstind[myid];
      MPI_Recv(x,count,MPI_DOUBLE,0,0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
   }
}
