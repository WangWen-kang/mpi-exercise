#include<mpi.h>
#include</usr/include/math.h>
#include<stdlib.h>
#include<stdio.h>
/* fuer den Aufruf von cblas_dgemv */
#ifdef USE_MKL
#include<mkl_cblas.h> 
#endif

void inimat(int size, double array[]);
void dgemv(int n1,int n2 ,double *a, int lda, double *x, double *y);

#define NSMPMX 100
#define N1MAX 512
#define N2MAX 32768
#define NMAX 16777216

int main(int argc,char **argv)
{ 
   int np, me;
   double *a, *x, *y;
   double t1, t2, speed, speedv, ts, su;
   int n1, n2, nt, l, nrep;
   int n1l, n1h, n1step, n2l, n2h, n2step;
   int i1, is1, i2, is2, lda;

   a = malloc(NMAX*sizeof(double));
   x = malloc(N1MAX*sizeof(double));
   y = malloc(N2MAX*sizeof(double));

   if(! (a && x && y) ) {
      printf("Error allocating memory.\n");
      return 1;
   }

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&np);
   MPI_Comm_rank(MPI_COMM_WORLD,&me); 

   printf("dgemv on one processor\n");
   printf("       n1       n2       nrep    total time   time/dgemv   speed \n");

   is2 = 1; n2l = 3; n2h = 15; n2step = 1;
   is1 = 0; n1l = 240;  n1h = 240; n1step = 40;
   lda = n1h;
   speedv = 100.; ts = .5;

   if (lda > N1MAX) {
      printf("lda too large \n");
      return 1;
   }

 
   for (i2 = n2l ; i2 <= n2h ; i2 = i2 + n2step)
      for (i1 = n1l ; i1 <= n1h ; i1 = i1 + n1step)
      {
         if (is1 == 0) 
            n1 = i1;
         else
            /* n1 = pow(2,i1); */
            n1 = 2<<i1;
         if (is2 == 0) 
            n2 = i2;
         else
            /* n2 = pow(2,i2); */
            n2 = 2<<i2;
     
         if (lda < n1) {
            printf("lda smaller than n2 \n");
            return 1;
         }
         if (n1 > N1MAX) {
            printf("n1 too large \n");
            return 1;
         }
         if (n2 > N2MAX) {
            printf("n2 too large \n");
            return 1;
         }

         nt = n1*n2;
         nrep = 10 + speedv*1.e6*ts/(2*nt);
         inimat(n2*lda,a);
         inimat(n1,x);

         t1 = MPI_Wtime();

         for (l = 1 ; l<= nrep; l++)
#ifdef USE_MKL
            cblas_dgemv(CblasColMajor, CblasTrans, n1, n2, 1. ,a, lda, x, 1., 0., y, 1);
#else
            dgemv(n1, n2, a, lda, x, y);
#endif

         t2 = MPI_Wtime()  - t1;

         speed = 0;
         if (t2 > 0.)
            speed = nrep*(long) 2*nt*1.0e-6 / t2;
         printf(" %8d  %8d  %8d  %e  %e  %f  \n",n1,n2,nrep,t2, t2/(2*nrep),speed);
      }

   su = 0.;
   for (l=0 ; l < n2;l++)
      su=su+y[l];
   printf("Kontrollsumme : %e\n",su);

   MPI_Finalize();
   return 0;
}

void inimat(int n1,double a[])
{
   int i;

   for ( i = 0 ; i <  n1; i++)
      a[i] = 1+i*0.1e-6;
   /*      printf("inimat: %e  %e\n",a[1],a[n1]);*/
}
