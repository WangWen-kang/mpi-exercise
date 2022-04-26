#include "mpi.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

void numapp(double lo, double hi, int n, double *res);

int main(argc,argv)
     int  argc;
     char *argv[];
{ 
  int np, me, tag=0;
  MPI_Status stat;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  MPI_Comm_rank(MPI_COMM_WORLD,&me);     

  double pia, hi, lo, width;
  double piexact = 3.14159265358979323846264338327950288419, res;
  int i, iw, ipw, n, nw, nin;


/*  number of strips and intervals */
      n = 100000000;
      nin = 50;
      int intv_l[nin+1]; 
      width = 1./((double) nin);
      if (me==0) {
         printf("\n calculation of pi using numerical integration of SQRT(1-x**2) \n number of strips %d \n number of subintervals    :  %d \n",n, nin);
      }
      if (me>0) {
/* ============  Worker-Teil (me>0) =======================*/
         res = 0.;
         MPI_Send(&res, 1, MPI_DOUBLE, 0, tag,MPI_COMM_WORLD);
         for (i = 1; i<= nin+1;i++){
            MPI_Recv( &iw, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat );
            if (iw > nin) break;
            lo = 1. -iw * width;
            hi = lo + width;
            numapp(lo,hi,n,&res);
            intv_l[i] = iw; nw = i;
            MPI_Send(&res, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
         }  
         printf("\n %3i >> ils: ",me);
         for (i = 1; i <= nw ; i++) printf(" %3i  ",intv_l[i]);
         printf("\n  ");
      }
/* ============  Farmer-Teil (me=0) =======================*/
      if (me == 0) {
         pia = 0.;
         for (iw = 1; iw <= nin+(np-1);iw++){
            MPI_Recv(&res, 1, MPI_DOUBLE, MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &stat);
            pia = pia + res;
            ipw = stat.MPI_SOURCE;
            MPI_Send(&iw, 1, MPI_INT, ipw, tag, MPI_COMM_WORLD);
         }
/*  output of results*/
         printf("\n pi exact          = %20.15f",piexact);
         printf("\n pi estimated      = %20.15f",pia);
         printf("\n error             = %20.3e\n",fabs(pia-piexact));
      }

  MPI_Finalize();
  return 0;
}
