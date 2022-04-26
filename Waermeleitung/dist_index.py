from mpi4py import MPI
import math
import time
import numpy as np

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def dist_index(n):
   nl = (n+nproc-1) / nproc
   firstind = np.zeros(nproc+1,dtype=int)
   firstind[0] = 0
   for ip in np.arange(1,nproc+1):
      firstind[ip] = min(n,firstind[ip-1]+nl)
   return firstind
