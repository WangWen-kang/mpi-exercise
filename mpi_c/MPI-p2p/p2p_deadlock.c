#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc,char **argv)
{
   int  np, me, vec[100000000], rvec[100000000], i, nl, ipto, ipfr, loop;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   nl = 1;
   for (loop = 1 ; loop < 9;loop++)
   {
      nl = 10*nl;
      for (i = 0 ; i < nl; i++)
      {
         vec[i] = me;
      }

      printf(" %3d>> size %10d start value %3d \n", me, nl, vec[0]);
      ipfr = (me+np-1)%np;
      ipto = (me+1)%np;

      MPI_Send( vec, nl, MPI_INTEGER, ipto, 0, MPI_COMM_WORLD);
      MPI_Recv( rvec, nl, MPI_INTEGER, ipfr, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf(" %3d>> size %10d value after ring shift %3d \n", me, nl, rvec[0]);

   }

   MPI_Finalize();
   return 0;
}
