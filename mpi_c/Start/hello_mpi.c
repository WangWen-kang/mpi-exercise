#include<mpi.h>
#include<stdio.h>

int main(int argc,char **argv)
{
  int np, me, resultlen, version, subversion;
  char name[40];

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  MPI_Get_processor_name(name, &resultlen);
  printf("hello %i %i %s \n",np,me,name);
  
  MPI_Finalize();
  return 0;
}
