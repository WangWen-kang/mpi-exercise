from mpi4py import MPI
import math
import time
import numpy as np

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def randaustausch(n1,n2l,a):
   ipr = (myid +1)
   if myid == nproc-1:
      ipr = MPI.PROC_NULL
   ipl = (myid -1)
   if myid == 0:
      ipl = MPI.PROC_NULL
   ase = np.ones(n1)
   arer = np.ones(n1)
   arel = a[1:n1+1,0]
   ase = a[1:n1+1,1]
   comm.Sendrecv(ase,dest=ipl,recvbuf=arel,source=ipl)
   a[1:n1+1,0] = arel
   arer = a[1:n1+1,n2l+1]
   ase = a[1:n1+1,n2l]
   comm.Sendrecv(ase,dest=ipr,recvbuf=arer,source=ipr)
   a[1:n1+1,n2l+1] = arer

   return a
