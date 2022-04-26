#include<mpi.h>
#include<stdio.h>
 
int work(int iw) {
   int myid;

   MPI_Comm_rank( MPI_COMM_WORLD, &myid);

   if ( (iw+myid) >10 )
      return -1;

   return 0;
}


int main(int argc,char **argv)
{ 
   int myid =0, nproc, iex, iexglob, il, iw;
	
   /* Start MPI */

   MPI_Init(&argc,&argv);
   MPI_Comm_size( MPI_COMM_WORLD, &nproc);
   MPI_Comm_rank( MPI_COMM_WORLD, &myid);

   if (myid == 0) {
      printf("iw ?\n");
      scanf("%d", &iw);
      /* iw = 10; */
   }

   MPI_Bcast(&iw,1,MPI_INT,0,MPI_COMM_WORLD);

   for ( il = 0 ;il<= iw; il++) {
      iex = work(il);
/*      printf("%d >> %d %d \n",myid,il,iex);*/
      MPI_Reduce(&iex,&iexglob,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
      MPI_Bcast(&iexglob,1,MPI_INT,0,MPI_COMM_WORLD);
      if (iexglob < 0) {
         printf("%d >> error exit %d %d %d \n",myid, il, iexglob, iex);
         il = iw+1;
      }
   }
   if (iexglob >= 0) printf("normal program end \n");
   MPI_Finalize ();
   return 0;
}
