#gather_obj.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

obj_loc = (10+myid,20+myid,30+myid)
print (myid, obj_loc)
obj_0 = comm.gather(obj_loc,root= 1)
if myid == 1:
   print (obj_0)
