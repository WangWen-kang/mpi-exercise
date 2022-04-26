#include<mpi.h>
#include<stdio.h>

/*
 +-------------+----------------+------------------------------------+
 |  parameter  |  input/output  |  description                       |
 +-------------+----------------+------------------------------------+
 |  n          |  input         |  indexrange to be distributed      |
 |  firstind   |  output        |  vector, firstind(ip) contains     |
 |             |                |  first global index on proc ip     |
 +-------------+----------------+------------------------------------+
 |                                                                   |
 |  Purpose of this subroutine is to calculate the value of the first|
 |  global index for each processor an to store these values into    |
 |  integer vector firtstind                                         |
 |                                                                   |
 +-------------------------------------------------------------------+
*/

void dist_index( int n, int *firstind ){

   int nproc;
   int nl, nr, ip;

   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   
   nl = (n+nproc-1) / nproc;
   firstind[0] = 0;
   for (ip = 1 ; ip <= nproc;ip++){
      firstind[ip] = firstind[ip-1]+nl;
      if (firstind[ip]>n) firstind[ip] = n;
   }
}
