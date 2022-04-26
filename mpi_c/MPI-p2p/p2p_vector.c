#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc,char **argv)
{
   int np, me, i, nsend, nrecv, startsend, startrecv;
   double vec[10];

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   for (i = 0 ; i < 10; i++)
   {
      vec[i] = me*(i+1); 
   }
   printf("\n %d >> initial:    vec=",me);
      for (i = 0 ; i < 10; i++) printf("%3.0f",vec[i]); printf("\n");

   if (me == 1)
   {  startsend = 4; nsend = 3;
      MPI_Send( &vec[startsend], nsend, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
   }

   if (me == 0)
   {  startrecv = 1; nrecv = 3;
      MPI_Recv(&vec[startrecv], nrecv, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("\n %d >> after recv: vec=",me);
      for (i = 0 ; i < 10; i++) printf("%3.0f",vec[i]); printf("\n");
   }
   MPI_Finalize();
   return 0;
}
