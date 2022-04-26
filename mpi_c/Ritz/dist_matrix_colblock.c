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
   int nproc, myid;
   int  firstind[NPROCMAX], i, ic, j, ip, ja, ncom, nl;
   MPI_Status stat;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

   dist_index( n, firstind );
   for (ip = nproc-1 ; ip >=0 ;ip--){
      ncom = m * ( firstind[ip+1] - firstind[ip] );
      nl = firstind[ip+1] - firstind[ip];

      /* Root first copies and then sends block */
      if (myid == 0){
	ja = firstind[ip];
        ic = 0;
        for (j = ja;j<ja+nl;j++){
	  for (i = 0;i<m;i++){
            al[ic] = a[i+j*m];
	    ic = ic+1;
	  }
	}
        
	if (ip>0) {
          MPI_Send( al, ncom, MPI_DOUBLE, ip, 0,  MPI_COMM_WORLD );
        }
      }
      if ((ip>0)&&(myid == ip)){
          MPI_Recv( al, ncom, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat ); 
      } 
   }

}
