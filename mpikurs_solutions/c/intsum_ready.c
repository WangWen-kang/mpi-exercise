#include<mpi.h>
#include<stdio.h>

int main(argc,argv)
int  argc; char *argv[];
{
   int np, me, i, n, suml, sum;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   MPI_Barrier( MPI_COMM_WORLD );

   if (me == 0) {
      printf("n ?\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
/* distribute the summation of integers from 1 to n
 over nproc tasks */
   suml = 0;
   for ( i = me+1; i <= n; i+=np){
      suml = suml +i;
   }
   MPI_Reduce(&suml,&sum,1,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD );

   if (me == 0) {
      printf("%d %d %d\n",n, sum, n*(n+1)/2);
   }

   MPI_Finalize();
   return 0;
}
