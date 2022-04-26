#include<math.h>
#include<stdio.h>
#include "mpi.h"


  main(argc,argv)
     int  argc;
     char *argv[];
{ 
  double pia, pial, del, app, xi;
  int n, np, me, i, ip; 
  MPI_Comm comm;
  double piexact = 3.14159265358979323846264338327950288419; 


/* 1) set up mpi environment */
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&np);
  MPI_Comm_rank(comm,&me);

/*  2) for me=0 : read the value for n from standard input*/
  if ( me == 0 ) {
     printf("\n  number of strips ?  \n");
     scanf("%d",&n);
     printf("\n calculation of pi using numerical integration of SQRT(1-x**2) \n number of intervals    :  %d \n number of processes    : %d \n",n,np);   }
/*  3) distribute n to all processes */
  if ( me == 0 ) {
//               np-1 SEND's of n in task 0
  }
  else  {
//               1 RECV of n in task 1,...,np-1
  }

//   4) starting from strip 1+me sum every np-th strip on task me

  del = 1./((double) n);
  app = 0.;
//  for (i ...){
    xi = (i+0.5)*del;
    app = app + sqrt(1.-xi*xi);
//  }
  pial = 4.*del*app;

//  5) collect all partial sums on task 0
  if (me = 0) {
     pia = pial;
//   np-1 RECV's of pial in task 0, adding up pial to pia
  }
  else  {
//        1 SEND of pial in all other tasks 
  }

/* 6) for me=0: write results to standard output */
  if ( me == 0 )  {
     printf("\n pi exact          = %20.15f",piexact);
     printf("\n pi estimated      = %20.15f",pia);
     printf("\n error             = %20.3e\n",fabs(pia-piexact));
  }

/* 7) exit from mpi */
  MPI_Finalize();
  return 0;
}



