#gather_vec.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

nl = 3
vec = np.ones(nl*nproc,dtype=np.float64)
vec_loc = np.arange(nl,dtype=np.float64)+10.*myid
#if myid == 1:
#   vec_loc = MPI.IN_PLACE
print (myid, vec_loc)
comm.Gather(vec_loc,vec,root= 1)
if myid == 1:
   print (vec[0:nproc*nl])
