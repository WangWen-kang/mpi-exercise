#scatter_vec.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

nl = 3
vec = np.ones(nl,dtype=np.float64)
vec_loc = np.arange(nl*nproc,dtype=np.float64)
if myid == 1:
   print (myid, vec_loc)
comm.Scatter(vec_loc,vec,root= 1)
print (myid,'   ',vec[0:nl])
