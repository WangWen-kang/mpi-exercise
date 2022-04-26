#include<mpi.h>
#include<math.h>
#include<stdio.h>

int main(int argc,char **argv)
{
  int np, me, nprime, ntest, nr, nd, i, ip;
  int numdiv[10];
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  nprime = 118845;
  if (me == 0) printf("prime number test for %i \n",nprime);
  ntest = sqrt((double)nprime)+1;
  nd = 0;
  for( i = 3+me; i<=ntest; i = i+np ){
     nr = nprime%i;
     if (nr == 0) {
        nd = nd+1;
        printf(" %i : %i  \n",me,i);
     }
  }
  MPI_Send(&nd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  if (me==0){ 
    for( ip = 0; ip<np; ip = ip+1 ){
      MPI_Recv(&numdiv[ip], 1, MPI_INT, ip, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    printf("number of divisors found in different processes: \n");
    for( ip = 0; ip<np; ip = ip+1 )  printf("%i ",numdiv[ip]);
    printf(" \n");
  }

  MPI_Finalize();
  return 0;
}
