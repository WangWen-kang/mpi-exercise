from mpi4py import MPI
import math
import time
import numpy as np
from mv_matmul import dgemv
from dist_index import dist_index
from dist_matrix_rowblock import dist_matrix_rowblock 
from global_vector import global_vector

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
a = np.zeros((n,n))
if myid == 0:
   for i in np.arange(n):
      for j in np.arange(i,n):
         a[i,j] = np.float64(i+j+2)/np.float64(j+1)
      for j in np.arange(i):
         a[i,j] = a[j,i]
firstind = dist_index(n)
nl = firstind[myid+1] - firstind[myid]
al = dist_matrix_rowblock(n,n,a)

#initialise start vector
y = np.ones(nl)   

evo = y[0]
evn = 0.0
istop = 0
tmv = 0.0
tcl = 0.0
tte = 0.0
tsc = 0.0

for i in np.arange(nloop):
   t1 = time.time() 
   x = global_vector(n,y)
   t2 = time.time()
   y = dgemv(1.,al,x)
   t3 = time.time()
   if myid == 0:
      evn = y[0]
   evn = comm.bcast(evn,root=0)
   t4 = time.time()
   res = (evn-evo)/evn
   evo = evn
   if abs(res) < eps:
      break
   y = y/evn
   t5 = time.time()
   tmv = tmv + t3 - t2
   tcl = tcl + t2 - t1
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

 
