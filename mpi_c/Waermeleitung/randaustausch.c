#include <mpi.h>
#include <stdio.h>

void randaustausch ( int n1, int n2, double **a ) {

   int ipu, ipo, nproc, myid;
   MPI_Status stat;


   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

#ifdef DEBUG
   printf("DEBUG: Randaustausch %d\n",myid);
#endif


   /* Kommunikation mit MPI_PROC_NULL hat keinen Effekt */
   ipo = myid - 1;
   if (myid == 0)
      ipo = MPI_PROC_NULL;
   ipu = myid + 1;
   if (myid == nproc-1)
      ipu = MPI_PROC_NULL;

   /* Kommunikation mit 'unterem' proc:
        Sende a[n1] als unsere letzte Zeile
        Empfange a[n1+1] mit dessen oberster Zeile */

   MPI_Sendrecv( &a[n1][1], n2, MPI_DOUBLE, ipu, 0,
                 &a[n1+1][1], n2, MPI_DOUBLE, ipu, 0, MPI_COMM_WORLD, &stat);


   /* Kommunikation mit 'oberem' proc:
       Sende a[1] als unsere erste Zeile
       Empfange a[0] mit dessen letzter Zeile */

   MPI_Sendrecv( &a[1][1], n2, MPI_DOUBLE, ipo, 0,
                 &a[0][1], n2, MPI_DOUBLE, ipo, 0, MPI_COMM_WORLD, &stat);


}
