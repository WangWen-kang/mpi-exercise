#gather_vec.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

# Define the partition vector dspls(0:nproc) for a global vector of size nglb
# and the size vector counts(0:nproc-1) with the sizes of local vectors
dspls = np.zeros(nproc+1,dtype=int)
counts = np.zeros(nproc,dtype=int)
dspls[0] = 0
for ip in np.arange(1,nproc+1):
   dspls[ip] = dspls[ip-1]+ 3+(ip-1)
   counts[ip-1] = dspls[ip] - dspls[ip-1]
nglb = dspls[nproc]

# Initialize the local vectors (such that v_glb(i) = i,i = 0,...,nglb-1):
v_lcl = np.zeros(counts[myid],dtype=np.float64)
for i in np.arange(0,counts[myid]):
   v_lcl[i] = dspls[myid]+i
print myid,'v_lcl',v_lcl

v_glb = np.zeros(nglb,dtype=np.float64)
#          Distribute local vectors v_lcl to their places in v_glb
# Copy local vector v_lcl to its place in local copy of v_glb */
for i in np.arange(0,counts[myid]):
   v_glb[dspls[myid]+i] = v_lcl[i]
# Broadcast local parts of v_glb to all processes
# Syntax:
# comm.Bcast(ar, root= 0) 
#   .......

print myid,'v_glb  ',v_glb
