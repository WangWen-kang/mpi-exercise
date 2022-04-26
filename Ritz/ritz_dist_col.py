from mpi4py import MPI
import math
import time
import numpy as np
from mv_matmul import dgemv
from dist_index import dist_index
from dist_matrix_colblock import dist_matrix_colblock 
from reduce_vector import reduce_vector

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

# matrix size
n = None
if myid == 0:
   print ("n = ?")
   n = int(input())
n = comm.bcast(n,root=0)
# maximal number of iterations
nloop = 100
# required precision
eps = 1.e-12
# matrix initialization
a = np.asfortranarray(np.zeros((n,n)))
if myid == 0:
   for i in np.arange(n):
      for j in np.arange(i,n):
         a[i,j] = np.float64(i+j+2)/np.float64(j+1)
      for j in np.arange(i):
         a[i,j] = a[j,i]

firstind = dist_index(n)
nl = firstind[myid+1] - firstind[myid]
al = dist_matrix_colblock(n,n,a)
#initialise start vector
x = np.ones(nl)   

evo = x[0]
evn = 0.0
istop = 0
tmv = 0.0
tcl = 0.0
tte = 0.0
tsc = 0.0

comm.Barrier()
for i in np.arange(nloop):
   t1 = MPI.Wtime() 
   y = dgemv(1.,al,x)
   t2 = MPI.Wtime()
   x = reduce_vector(n,y)
   t3 =MPI.Wtime()
   if myid == 0:
      evn = x[0]
   evn = comm.bcast(evn,root=0)
   t4 = MPI.Wtime()
   res = (evn-evo)/evn
   evo = evn
   if abs(res) < eps:
      break
   x = x/evn
   t5 = MPI.Wtime()
   tmv = tmv + t2 - t1
   tcl = tcl + t3 - t2
   tte = tte + t4 - t3
   tsc = tsc + t5 - t4

comm.Barrier()
flops = 1.e-6 * float(2 * (i+1) * n * n) / (tmv+tcl)
if myid == 0:
   print ('-------------------------------')
   print ('matrix dimension is ',n,'x',n)
   print ('largest eigenvalue by Ritz method on',nproc,'processors')
   print ('eigenvalue after',i,'iterations : ',evn)
   print ('compute time : %8.5f sec, communication time : %8.5f sec ' % (tmv,  tcl))
   print ('speed : %8.2f MFlop/s' % flops)

 
