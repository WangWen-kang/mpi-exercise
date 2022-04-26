#include<mpi.h>
#include<stdio.h>

int main(argc,argv)
int  argc; char *argv[];
{
   int np, me, msg, ip, i;
   double tm, t0, t1, tbmax;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   MPI_Barrier( MPI_COMM_WORLD );

   tm = MPI_Wtime();
   t0 = MPI_Wtime();
   msg = 1;
/*! exchange messages between task 0 and all other tasks
! then send messages from task 0 to all other tasks
!   syntax:
        MPI_Send( &sbuf, 1, MPI_INT, taskid, tagnumber, MPI_COMM_WORLD);
        MPI_Recv( &rbuf, 1, MPI_INT, taskid, tagnumber, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
*/
   for (ip=1;ip<np;ip++){
      if (me == ip) {
        MPI_Send( &msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
      if (me == 0){
        MPI_Recv( &msg, 1, MPI_INT, ip, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      }
   }
   for (ip=1;ip<np;ip++){
      if (me == 0){
        MPI_Send( &msg, 1, MPI_INT, ip, 0, MPI_COMM_WORLD);
      }
      if (me == ip) {
        MPI_Recv( &msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      }
   }
   t1 = MPI_Wtime() - t0;
   MPI_Reduce( &t1, &tbmax, 1, MPI_DOUBLE, MPI_MAX, 0,  MPI_COMM_WORLD );

   if (me == 0) {
      printf("Barriere fuer %i Prozessoren dauert : %e\n",np, tbmax);
   }

   MPI_Finalize();
   return 0;
}
