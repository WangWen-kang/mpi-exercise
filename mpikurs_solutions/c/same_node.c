#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
    int me, np, ip;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &me );
    MPI_Comm_size( MPI_COMM_WORLD, &np );
    int namelen = 60, len;
    char names[np][namelen],name[namelen];
    int same[np];
    MPI_Get_processor_name( name, &len );
    MPI_Allgather( name, namelen, MPI_CHAR, names, namelen, MPI_CHAR, MPI_COMM_WORLD );
    int ct = -1, dev_nbr;
    for (ip=0; ip<np; ip++) {
      if (strcmp(names[me],names[ip])==0){
         ct = ct + 1;
         same[ct] = ip;
//         if (me == ip) dev_nbr = ct;
      }
    }
  for (ip=0;ip<=ct;ip++) printf("|| %i> tasks on same node: %i \n",me,same[ip]);
  
  MPI_Finalize();
}



