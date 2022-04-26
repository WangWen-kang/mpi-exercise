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
 |             |                |  nl rows of a                      |
 +-------------+----------------+------------------------------------+
 |                                                                   |
 |  Purpose of this subroutine is to distribute a matrix a           |
 |  over all processors (row-wise)                                   |
 |                                                                   |
 ++-+--+----+----+----+----+----+----+----+----+----+----+----+----+-+
*/

void dist_matrix_rowblock( int m, int n, double *a, double *al ){

   int nproc, myid;
   int  firstind[NPROCMAX], i, ic, j, ip, ia, ncom, nl;
   MPI_Datatype rowblock;
 
   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 
 
   dist_index( m, firstind );

   /* Now distribute blocks from root to the others */
   if (myid == 0){
         nl = firstind[1] - firstind[0];
         ic = 0;
         for (j = 0;j<n;j++){
            for (i = 0;i< nl;i++){
               al[ic] = a[i+j*m];
               ic = ic+1;
            }
         }

      for (ip = nproc-1 ; ip >= 1 ;ip--){
         nl = firstind[ip+1] - firstind[ip];
         MPI_Type_vector(n,nl,m,MPI_DOUBLE, &rowblock);
         MPI_Type_commit(&rowblock);
         ia = firstind[ip];
         MPI_Send( &a[ia], 1, rowblock, ip, 0, MPI_COMM_WORLD );
         MPI_Type_free(&rowblock);
      }
   }
   else{
      ncom = n * ( firstind[myid+1] - firstind[myid] );         
      MPI_Recv(al, ncom, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE ); 
   }
}

