#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ritz.h"

/*
 +-------------+----------------+------------------------------------+
 |  parameter  |  input/output  |  description                       |
 +-------------+----------------+------------------------------------+
 |  m          |  input         |  number of rows of a and al        |
 |  n          |  input         |  number of columns of a            |
 |  a          |  input         |  global matrix                     |
 |  al         |  output        |  local matrix, containing          |
 |             |                |  nl columns of a                   |
 +-------------+----------------+------------------------------------+
 |                                                                   |
 |  Purpose of this subroutine is to distribute a matrix a           |
 |  over all processors (column-wise)                                | 
 |                                                                   |
 ++-+--+----+----+----+----+----+----+----+----+----+----+----+----+-+
*/


void dist_matrix_colblock( int m, int n, double *a, double *al )
{
   int nproc, myid, ip;
   int  firstind[NPROCMAX], counts[NPROCMAX],displs[NPROCMAX];

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   dist_index( n, firstind );
   int cnt = m*(firstind[myid+1] - firstind[myid]);
   for (ip = 0 ; ip < nproc ;ip++){
         displs[ip] = m * firstind[ip];
         counts[ip] = m * ( firstind[ip+1] - firstind[ip] );
   }
   MPI_Scatterv(a, counts, displs, MPI_DOUBLE, al, cnt, MPI_DOUBLE,0, MPI_COMM_WORLD );
}
