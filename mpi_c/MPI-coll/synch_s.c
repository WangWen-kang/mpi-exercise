#include<mpi.h>
#include<stdio.h>

int main(argc,argv)
int  argc; char *argv[];
{
   int np, me, sbuf, rbuf, ip, i;
   double tm, t0, t1, tbmax;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   MPI_Barrier( MPI_COMM_WORLD );

   tm = MPI_Wtime();
   t0 = MPI_Wtime();
   sbuf = 1;
/*! Example implementation of a barrier (sequential access):
!   step 1.) all tasks except task 0 send a message to task 0
!   step 2.) task 0 sends a message to all other tasks
!   syntax:
        MPI_Send( &sbuf, 1, MPI_INT, dest, tagnumber, MPI_COMM_WORLD);
        MPI_Recv( &rbuf, 1, MPI_INT, srce, tagnumber, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
*/
   }


   t1 = MPI_Wtime() - t0;
   MPI_Reduce( &t1, &tbmax, 1, MPI_DOUBLE, MPI_MAX, 0,  MPI_COMM_WORLD );

   if (me == 0) {
      printf("Barriere fuer %i Prozessoren dauert : %e\n",np, tbmax);
   }

   MPI_Finalize();
   return 0;
}
