#include "mpi.h"
#include<stdio.h>

int main(int argc,char **argv)
{
   int np, me, i;
   int nmax = 100;
   double t[nmax], tick, diff;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   for (i = 0 ; i < nmax; i++)
   {
   t[i] = MPI_Wtime();
   }

   tick = MPI_Wtick();

   printf("precision of Wtime : %e sec\n",tick);
   printf("time for Wtime     : %e sec\n",(t[nmax-1]-t[2])/(nmax-2));

   MPI_Finalize();
   return 0;
}
