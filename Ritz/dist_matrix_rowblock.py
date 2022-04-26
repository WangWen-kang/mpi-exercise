from mpi4py import MPI
import math
import time
import numpy as np
from dist_index import dist_index 
comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def dist_matrix_rowblock( m, n, a ):
   firstind = dist_index(m)
   ia = firstind[myid]
   ie = firstind[myid+1]
   nl = ie -ia
   al = np.empty((nl,n))
   if myid == 0:
      for ip in range(nproc):
         ia = firstind[ip]
         ie = firstind[ip+1]
         nl = ie -ia
         if ip == 0:
            al = a[ia:ie,0:n]
         else:
            comm.Send(a[ia:ie,0:n], dest= ip)
   else:
      comm.Recv(al, source=0)
   return al

