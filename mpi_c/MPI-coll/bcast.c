#include<mpi.h>
#include<stdio.h>

int main(argc,argv)
int  argc; char *argv[];
{
   int np, me, ip;
   MPI_Comm com=MPI_COMM_WORLD;
   int niter; float val;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(com,&np);
   MPI_Comm_rank(com,&me);

/* for me=0 : read input values  from standard input*/
  if ( me == 0 ) {
     printf("\n  number of iterations (int) ?  \n");
     scanf("%d",&niter);
     printf("\n start value (real) ?  \n");
     scanf("%f",&val);

     printf("\n input values %d , %f \n", niter,val);   }

  if ( me == 0 ) {
     for ( ip=1;ip<np;ip++ ) {
         MPI_Send( &niter, 1, MPI_INT, ip, 0, com );
         MPI_Send( &val, 1, MPI_FLOAT, ip, 1, com ); }
  }
  else  {
     MPI_Recv( &niter, 1, MPI_INT, 0, 0, com, MPI_STATUS_IGNORE );
     MPI_Recv( &val, 1, MPI_FLOAT, 0, 1, com, MPI_STATUS_IGNORE );
  }
/*  Use MPI_Bcast for distributing input values
   Syntax:
   MPI_Bcast( void *buf, int count, MPI_Type datatype, int root, MPI_Comm comm )
*/
   printf("\n on proc. %d input values: %d , %f \n",me,niter,val); 

   MPI_Finalize();
   return 0;
}
