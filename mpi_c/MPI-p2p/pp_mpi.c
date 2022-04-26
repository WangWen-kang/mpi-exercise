#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

/*
  -------------------------------------------------------------------+
  --      pingpong test to evaluate communication
  --      between two processors
  --      If run on nproc processors, pingpong evaluates
  --      the communication between processor 0 and processor ip,
  --      ip = 1,...,nproc-1
  -------------------------------------------------------------------+
*/

#define MAXPROC 128

int main(int argc,char **argv)
{
   int maxlen = 2000000;
   int i, il, len, irep, lrep, iw, ip;
   int rtag, stag;
   double *vec, *snd_vec, *rcv_vec;
   double lat[MAXPROC], speedmax[MAXPROC];
   double t0, speedg, tmin, tg, dt, speed, tlat;
   int nproc, myid;

   MPI_Status status;

   vec = malloc( maxlen * sizeof(double) );
   snd_vec = malloc( maxlen * sizeof(double) );
   rcv_vec = malloc( maxlen * sizeof(double) );

   if(! (vec && snd_vec && rcv_vec)) {
      printf("Error allocating memory.");
      return 1;
   }
   

   /* Start MPI */

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   if ( nproc > MAXPROC ) {
      if ( myid == 0 ) {
         printf("\nnproc = %i  too large",nproc);
	 printf("\nmaximal nproc is MAXPROC");
      }
      return 0; /* Da sonst segmentation fault beim Setzen von speedmax */
   }

   /* If first process, set environment */

   if ( myid == 0 )
   {
      /* Input  */
      iw = 20;
      tlat = 0.0000008;
      speedg = 1000000000.0;
      tmin = 0.1;

      printf("\npingpong communcation test on %i processors \n ", nproc );
      printf(" %d %f %f  %f \n\n\n",iw, tlat, speedg, tmin);
      printf("       size         reps     ttot       latency     speed \n");
      printf("      [Bytes]               [secs]      [secs]    [MB/secs]\n");
   }

   /* communicate parameters */

   MPI_Bcast(&iw,1,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Bcast(&tlat,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
   MPI_Bcast(&speedg,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
   MPI_Bcast(&tmin,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

   for (ip = 1; ip <= nproc - 1; ip++) {
      speedmax[ip-1] = 0;

      for (il = 0; il <= iw; il++) {

         len = 2<<il;
         if (len <= maxlen){
	    tg = 2*tlat + 16 * len / speedg;
	    lrep = 1 + tmin / tg;
	    for (i = 1; i<=len; i++) {
	       snd_vec[i-1] = i-1; rcv_vec[i-1] = 0;}

            /* allover synchronization */

            MPI_Barrier(MPI_COMM_WORLD);
            t0 = MPI_Wtime();

            /* communicate data */

	    for (irep = 1; irep <= lrep; irep++) {
	       stag = 1000 + 2*irep;
	       rtag = 1001 + 2*irep;
	       if (myid == 0) {
	          MPI_Send(snd_vec,len,MPI_DOUBLE,ip,stag,MPI_COMM_WORLD);
                  MPI_Recv(rcv_vec,len,MPI_DOUBLE,ip,rtag,MPI_COMM_WORLD,&status);
               }
	       else if(myid == ip) {
	          MPI_Recv(vec,len,MPI_DOUBLE,0,stag,MPI_COMM_WORLD,&status);
                  MPI_Send(vec,len,MPI_DOUBLE,0,rtag,MPI_COMM_WORLD);
	       }
	    }

            dt = MPI_Wtime() - t0;

            /* Test for correct communication */

	    if (myid == 0) {
	       for (i = 1; i <= len; i++) {
	          if (rcv_vec[i-1] != snd_vec[i-1]) {
                     printf("communication error %i %e %e\n", i, rcv_vec[i-1], snd_vec[i-1]);
                     i = len;
		  }
	       }
	    }

            /* Output of communication speed */

	    if (myid == 0) {
	       if (dt == 0)
                  speed = 0;
	       else
	          speed = 16* len *lrep/ dt *1.e-9;
	       if (il == 0)
	          lat[ip-1] = dt/(2*lrep-1);
	       if (speed > speedmax[ip-1])
	          speedmax[ip-1] = speed;
               printf(" %10d  %10d   %f    %f  %10.3f \n",8*len,lrep,dt,dt/(double)(2*lrep),speed);
               /* Obige Ausgaben muessten noch in ein entsprechendes
                  Format gebracht werden */
	    }
	 }
      }
   }
   if (myid == 0) {
      printf("\n****************\n");
      printf("pingpong summary");
      printf("\n****************\n");
      printf("number of procs %i\n", nproc);
      printf("communication between proc 0 and proc ip:\n");
      printf("ip\tlatency[s]\tbandwith[MB/s]\n");
      for (ip = 1; ip <= nproc-1; ip++) {
         printf("%i\t%10.2e \t%10.2f\n", ip, lat[ip-1], speedmax[ip-1]);
      }
   }

   MPI_Finalize();
   return 0;
}
