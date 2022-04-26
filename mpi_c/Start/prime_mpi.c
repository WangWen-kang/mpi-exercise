#include<mpi.h>
#include<math.h>
#include<stdio.h>

int main(int argc,char **argv)
{
  int np, me, nprime, ntest, nr, nd, i;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  nprime = 118845;
  if (me == 0) printf("prime number test for %i \n",nprime);
  ntest = sqrt((double)nprime)+1;
  nd = 0;
  for( i = 3; i<=ntest; i = i+1 ){
     nr = nprime%i;
     if (nr == 0) {
        nd = nd+1;
        printf(" %i : %i  \n",me,i);
     }
  }
  printf("task %i found %i divisors \n",me,nd);
  
  MPI_Finalize();
  return 0;
}
