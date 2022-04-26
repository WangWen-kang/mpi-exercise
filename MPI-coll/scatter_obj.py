#scatter_obj.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()
obj_0 = None
if myid == 1:
   obj_0 = [(0,1),(2,3),(4,5),(6,7),(8,9),(10,11)]
   print (myid, obj_0)
obj_loc = comm.scatter(obj_0,root= 1)
#obj_loc = comm.scatter(sendobj=obj_0,recvobj=None,root= 1)
print (myid, obj_loc)
