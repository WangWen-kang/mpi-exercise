from mpi4py import MPI
import math
import time
import numpy as np
from dist_index import dist_index 

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def global_vector(n,y):
   firstind = dist_index(n)
   counts = np.zeros(nproc,dtype=int)
   for ip in np.arange(1,nproc+1):
      counts[ip-1] = firstind[ip] - firstind[ip-1]
   x = np.zeros(n,dtype=np.float64)
   comm.Allgatherv( y, [x,counts,firstind[0:nproc],MPI.DOUBLE])
   return x
