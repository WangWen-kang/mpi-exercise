from mpi4py import MPI
import math
import time
import numpy as np
from dist_index import dist_index 

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

def reduce_vector(n,y):
   ys = np.zeros(n)
   comm.Reduce(y,ys,op=MPI.SUM,root = 0) 
   firstind = dist_index(n)
   if  myid == 0:
      count = firstind[1] - firstind[0]
      x = ys[0:count]
      for ip in range(1,nproc):
         ia = firstind[ip]
         count = firstind[ip+1] - firstind[ip]
         comm.Send(ys[ia:ia+count],dest=ip)
   else:
      count = firstind[myid+1] - firstind[myid]
      x = np.zeros(count)
      comm.Recv(x[0:count],source=0)
   return x
