from mpi4py import MPI
import math
import time
import numpy as np
from dist_index import dist_index 
comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def dist_matrix_colblock( m, n, a ):
   firstind = dist_index(n)
   ia = firstind[myid]
   ie = firstind[myid+1]
   nl = ie -ia
   al = np.asfortranarray(np.empty((m,nl)))
   if myid == 0:
      for ip in range(nproc):
         ia = firstind[ip]
         ie = firstind[ip+1]
         nl = ie -ia
         asend = np.asfortranarray(np.empty((m,nl)))
         asend = a[0:m,ia:ie]
         if ip == 0:
            al = a[0:m,ia:ie]
         else:
            comm.Send(a[0:m,ia:ie], dest= ip)
   else:
      comm.Recv(al, source=0)
   return al

